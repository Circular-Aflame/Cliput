#include "timelinewidget.h"
#include "../processor.h"
#include "../algor.h"
#include "../waiting.h"
#include "../timeline/effectdialog.h"
#include <QGraphicsItem>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QList>
#include <QCoreApplication>

TimelineWidget::TimelineWidget(QWidget*parent)
    :QDockWidget(parent)
{
    this->setWindowTitle("Timeline");
    w=new QWidget(this);
    this->setWidget(w);
    layout =new QVBoxLayout(w);
    timer=new QTimer(this);

    setCachePath("");
    addToolbar();
    addview();
}

void TimelineWidget::addview()
{
    //绘图初始化
    scene=new TimelineScene();
    view=new TimelineView(scene);
    connect(scene,SIGNAL(review()),view->viewport(),SLOT(update()));
    layout->addWidget(view);
    scene->setSceneRect(0,0,scene->lastms/scene->step,10*scene->trackheight);
    marker=new TimeMarker(scene);
    timer->setInterval(marker->interval);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    connect(marker,SIGNAL(valueChanged(int)),currentBox,SLOT(setValue(int)));
    connect(scene,SIGNAL(markerChanged(int)),marker,SLOT(setValue(int)));
    connect(scene,SIGNAL(markerChanged(int)),marker,SIGNAL(positionMoved(int)));
    connect(marker,SIGNAL(valueChanged(int)),this,SLOT(previewUpdate()));
    connect(marker,SIGNAL(positionMoved(int)),this,SIGNAL(previewStop()));
    connect(marker,SIGNAL(positionMoved(int)),this,SLOT(previewUpdate()));

    timescale=new TimeScale(scene);
    scene->addItem(timescale);
    scene->addItem(marker);
}

void TimelineWidget::addToolbar()
{
    toolbar=new QToolBar(this);
    toolbar->setAllowedAreas(Qt::TopToolBarArea);
    layout->addWidget(toolbar);

    toolbar->addAction("-",this,&TimelineWidget::onActionDeleteTrack);
    toolbar->addAction("][",this,&TimelineWidget::onActionCut);

    toolbar->addAction("ClipOut",this,&TimelineWidget::onActionClipout);
    toolbar->addAction("Combine",this,&TimelineWidget::onActionCombine);
    toolbar->addAction("VideoOut",this,&TimelineWidget::onActionVideoout);
    toolbar->addAction("Effect",this,&TimelineWidget::onAddEffect);
    toolbar->addAction("CaptionSRT",this,&TimelineWidget::addSRTCaption);

    currentBox=new Timebox(this);
    currentBox->setSingleStep(1000);
    currentBox->setMinimum(0);
    currentBox->setMaximum(1000000000);
    currentBox->setValue(6000);
    toolbar->addWidget(currentBox);
    connect(currentBox,SIGNAL(editingFinished()),this,SLOT(onMarkerChanged()));

}

void TimelineWidget::setCachePath(QString path)
{
    if(path=="")
    {
        if(QDir(QDir::currentPath()).mkpath("cache"))
        {

            cachePath=QDir::currentPath()+"/cache";
        }
    }
    else
    {
        cachePath=path;
    }
}

void TimelineWidget::onAddEffect()
{
    QList<TrackItem*>items=scene->selectedTracks();
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Picture)
        {
            PictureTrack*it=(PictureTrack*)item;
            EffectDialog effectdialog(this,it);
            effectdialog.exec();
        }
    }
}

void TimelineWidget::onActionCombine()
{
    QList<TrackItem*> items=scene->selectedTracks();
    if(items.length()<=1)
    {
        return;
    }
    //保留缓存文件路径
    QList<QString> tempfiles;
    //保存路径
    int num=1;
    QString filename=cachePath+QString("/combine-%1.mp4").arg(num);
    while(QFile::exists(filename))
    {
        filename=cachePath+QString("/combine-%1.mp4").arg(++num);
    }
    //等待提示
    Waiting wait(this);
    wait.show();
    QCoreApplication::processEvents();
    //处理
    int count=0;
    QFile file(cachePath+"/concat.txt");
    file.open(QIODevice::WriteOnly);
    file.close();
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"!!!bug!!!";
        return;
    }
    tempfiles.append(file.fileName());
    QTextStream stream(&file);
    //片段裁剪转码
    int dur=0;
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Video)
        {
            count++;
            VideoTrack *vt=(VideoTrack*)item;
            dur+=item->exportms-item->importms;
            Processor pro(this);
            QStringList arguments;
            QString name=QString("%1/%2.%3").arg(cachePath).arg(count).arg(vt->text.split(".")[1]);
            arguments<<"-ss"<<Mstime::toHMSM(vt->startms)<<"-to"<<Mstime::toHMSM(vt->endms);
            arguments<<"-i"<<vt->url<<"-c"<<"copy"<<"-y"<<name;
            if(pro.process(arguments))
            {
                arguments.clear();
                arguments<<"-i"<<name<<"-vcodec"<<"copy"<<"-acodec"<<"copy"<<"-vbsf"<<"h264_mp4toannexb"<<"-y"<<name.split(".")[0]+".ts";
                if(pro.process(arguments))
                {
                    stream<<"file "<<"'"<<name.split(".")[0]<<".ts'"<<"\n";
                    tempfiles.append(name);
                    tempfiles.append(name.split(".")[0]+".ts");
                }
            }
        }
    }
    file.close();
    //合成
    Processor pro(this);
    QStringList arguments;
    arguments<<"-f"<<"concat"<<"-safe"<<"0"<<"-i"<<file.fileName()<<"-strict"<<"-2"<<"-y"<<filename;
    if(pro.process(arguments))
    {
        wait.close();
        emit newClipout(filename);
    }
    //清除缓存
    for(auto tempfile:tempfiles)
    {
        if(QFile(tempfile).remove())
        {
        }
    }
    qDebug()<<"cache cleared";
    VideoTrack *v=new VideoTrack(QString("combine-%1").arg(num),dur,scene,items[0]->importms,items[0]->importms+dur);
    v->setUrl(filename);
    v->setWithAudio(true);
    v->update();
    scene->addItem(v);
    videoTrack.push_back(v);
    scene->refresh();
    onActionDeleteTrack();
}

//合并视频
void TimelineWidget::onActionVideoout()
{
    QList<TrackItem*> items=scene->allTracks();
    if(items.length()==0)
    {
        return;
    }
    //保留缓存文件路径
    QList<QString> tempfiles;
    //保存路径
    QString filename=QFileDialog::getSaveFileName(this,
                                                  tr("Save video"),
                                                  "nicecut",
                                                  tr("*.mp4;;*.mkv"));
    if(filename.isEmpty())
    {
        return;
    }
    //等待提示
    Waiting wait(this);
    wait.show();
    QCoreApplication::processEvents();
    //处理
    Processor pro(this);
    QStringList arguments;
    int count=0;
    QFile file(cachePath+"/concat.txt");
    file.open(QIODevice::WriteOnly);
    file.close();
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"!!!bug!!!";
        return;
    }
    tempfiles.append(file.fileName());
    QTextStream stream(&file);
    //片段裁剪转码
    int dur=0;
    QString concatname="concat:";
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Video)
        {
            count++;
            VideoTrack *vt=(VideoTrack*)item;
            if(vt->importms<dur)
            {
                count--;
                continue;
            }
            if(vt->importms>dur)
            {
                //生成黑幕
                QString interval=Mstime::toHMSM(vt->importms-dur);
                QString name=QString("%1/%2.mp4").arg(cachePath).arg(count);
                arguments.clear();
                arguments << "-f" << "lavfi" << "-i" << "color=black:s=1920x1080:r=24000/1001" << "-f" << "lavfi" << "-i" << "anullsrc" << "-ar"
                          << "48000" << "-ac" << "2" << "-t" << QString::number((vt->importms-dur)/1000) << "-y" << name;
                if(pro.process(arguments))
                {
                    arguments.clear();
                    arguments<<"-i"<<name<<"-vcodec"<<"copy"<<"-acodec"<<"copy"<<"-vbsf"<<"h264_mp4toannexb"<<"-y"<<name.split(".")[0]+".ts";
                    if(pro.process(arguments))
                    {
                        stream<<"file"<<" '"<<name.split(".")[0]<<".ts'\n";
                        qDebug()<<name;
                        tempfiles.append(name);
                        tempfiles.append(name.split(".")[0]+".ts");
                        concatname+=name.split(".")[0]+".ts|";
                    }
                }
                count++;
                dur=vt->importms;
            }
            dur=vt->exportms;
            QString name=QString("%1/%2.%4").arg(cachePath).arg(count).arg(vt->text.split(".")[1]);
            arguments.clear();
            arguments<<"-ss"<<Mstime::toHMSM(vt->startms)<<"-to"<<Mstime::toHMSM(vt->endms)<<"-i"<<vt->url<<"-c"<<"copy"<<"-y"<<name;
            if(pro.process(arguments))
            {
                arguments.clear();
                arguments<<"-i"<<name<<"-vcodec"<<"copy"<<"-acodec"<<"copy"<<"-vbsf"<<"h264_mp4toannexb"<<"-y"<<name.split(".")[0]+".ts";
                if(pro.process(arguments))
                {
                    stream<<"file "<<"'"<<name.split(".")[0]<<".ts'"<<"\n";
                    tempfiles.append(name);
                    tempfiles.append(name.split(".")[0]+".ts");
                    concatname+=name.split(".")[0]+".ts|";
                }
            }
        }
    }
    file.close();
    //合成
    //小圆失效
    int temp=0;
    QString tempname=cachePath+QString("/temp%1.mp4").arg(temp);
    tempfiles.append(tempname);
    arguments.clear();
    arguments<<"-f"<<"concat"<<"-safe"<<"0"<<"-i"<<file.fileName()<<"-strict"<<"-2"<<"-vcodec"<<"copy"<<"-absf"<<"aac_adtstoasc"<<"-y"<<tempname;
    if(pro.process(arguments))
    {
        qDebug()<<"ok";
    }
    QString lastfile=tempname;
    //添加字幕及图片
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Picture)
        {
            tempname=cachePath+QString("/temp%1.mp4").arg(++temp);
            tempfiles.append(tempname);
            PictureTrack*it=(PictureTrack*)item;
            arguments.clear();
            arguments<<"-i"<<lastfile<<"-i"<<item->url<<"-filter_complex";
            QString timeline;
//            if(it->scaling)
//            {
//                timeline+=QString("[1:v]scale=(%2*(t/%1-1)+%3*t/%1):").arg((it->exportms-it->importms)/1000).arg(it->scalstart.width()).arg(it->scalend.width())
//                        +QString("y=(%2*(t/%1-1)+%3*t/%1)").arg((it->exportms-it->importms)/1000).arg(it->scalstart.width()).arg(it->scalend.height())
//                        +"[s];";
//            }
//            else
//            {
//                timeline+=QString("[1:v]scale=")+QString("%1:%2").arg(it->pic->width).arg(it->pic->height)
//                        +"[s];";
//            }
            timeline+=QString("[1:v]scale=")+QString("%1:%2").arg(it->pic->width).arg(it->pic->height)
                                    +"[s];";
            if(it->translation)
            {
                timeline+=QString("[0:v][s]overlay=x=(%2*(t/%1-1)+%3*t/%1):").arg((it->exportms-it->importms)/1000).arg(it->transtart.x()).arg(it->transend.x())
                        +QString("y=(%2*(t/%1-1)+%3*t/%1)").arg((it->exportms-it->importms)/1000).arg(it->transtart.y()).arg(it->transend.y());
            }
            else
            {
                timeline+=QString("[0:v][s]overlay=%1:%2").arg(it->pic->x()).arg(it->pic->y());
            }

            timeline+=QString(":enable='between(t,%3,%4)'").arg(it->importms/1000).arg(it->exportms/1000);
            arguments<<timeline<<"-y"<<tempname;

            qDebug()<<arguments.join(" ");
            if(pro.process(arguments))
            {
                qDebug()<<tempname<<"done";
                lastfile=tempname;
            }

        }
        else if(item->tracktype==TrackItem::Caption)
        {
            tempname=cachePath+QString("/temp%1.mp4").arg(++temp);
            tempfiles.append(tempname);
            CaptionTrack*it=(CaptionTrack*)item;
            arguments.clear();
            arguments<<"-i"<<lastfile<<"-vf";
            QString timeline;
            timeline+=QString("drawtext=fontcolor=white:fontsize=40:fontfile=msyh.ttf:text='%1':x=%2:y=%3").arg(it->sub->toPlainText()).arg(it->sub->x()).arg(it->sub->y());
            timeline+=QString(":enable='between(t,%3,%4)'").arg(it->importms/1000).arg(it->exportms/1000);
            arguments<<timeline<<"-y"<<tempname;

            qDebug()<<arguments.join(" ");
            if(pro.process(arguments))
            {
                qDebug()<<tempname<<"done";
                lastfile=tempname;
            }
        }
    }
    arguments.clear();
    arguments<<"-i"<<lastfile<<"-c"<<"copy"<<"-y"<<filename;
    if(pro.process(arguments))
    {
        wait.close();
        emit newClipout(filename);
    }
    //清除缓存
    for(auto tempfile:tempfiles)
    {
        if(QFile(tempfile).remove())
        {
            qDebug()<<"clean "<<tempfile;
        }
    }
    qDebug()<<"cache cleared";
}

//片段导出
void TimelineWidget::onActionClipout()
{
    QList<TrackItem*> items=scene->selectedTracks();
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Video)
        {
            VideoTrack *vt=(VideoTrack*)item;
            Processor pro(this);
            QStringList arguments;
            int count=1;
            QString name=QString("%1/%2-cut-%3.%4").arg(cachePath).arg(vt->text.split(".")[0]).arg(count).arg(vt->text.split(".")[1]);
            while(QFile::exists(name))
            {
                name=QString("%1/%2-cut-%3.%4").arg(cachePath).arg(vt->text.split(".")[0]).arg(++count).arg(vt->text.split(".")[1]);
            }
            arguments<<"-ss"<<Mstime::toHMSM(vt->startms)<<"-to"<<Mstime::toHMSM(vt->endms)
                    <<"-i"<<vt->url<<"-c"<<"copy"<<name;
            if(pro.process(arguments))
            {
                emit newClipout(name);
            }
        }
    }
}

void TimelineWidget::previewUpdate()
{
    QList<TrackItem*> items=scene->allTracks();
    QList<TrackItem*> showitems;
    QList<TrackItem*> outitems;
    for(auto item:items)
    {
        if(item->importms<=marker->val()&&item->exportms>=marker->val())
        {
            item->markertime=marker->val();
            showitems.append(item);
        }
        else
        {
            if(item->tracktype==TrackItem::Caption||item->tracktype==TrackItem::Picture)
            {
                outitems.append(item);
            }
        }
    }
    emit tracktoPreview(showitems);
    emit deleteItems(outitems);
}

void TimelineWidget::onMarkerChanged()
{
    marker->setValue(currentBox->value());
    scene->refresh();
    emit previewStop();
}

void TimelineWidget::onTimeout()
{
    marker->setValue(marker->val()+marker->interval);
}

void TimelineWidget::previewPlay(bool isPlaying)
{
    if(isPlaying)
    {
        timer->start();
    }
    else
    {
        timer->stop();
    }
}

void TimelineWidget::onActionCut()
{
    QList<TrackItem*> items=scene->selectedTracks();
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Video)
        {
            VideoTrack *vt=(VideoTrack*)item;
            VideoTrack *clip;
            if((clip=vt->cut(marker->val()))!=nullptr)
            {
                videoTrack.push_back(clip);
                scene->addItem(clip);
            }
        }
        else if(item->tracktype==TrackItem::Audio)
        {
            AudioTrack *vt=(AudioTrack*)item;
            AudioTrack *clip;
            if((clip=vt->cut(marker->val()))!=nullptr)
            {
                audioTrack.push_back(clip);
                scene->addItem(clip);
            }
        }
    }
    scene->refresh();
}

void TimelineWidget::onActionDeleteTrack()
{
    QList<TrackItem*> items=scene->selectedTracks();
    QList<TrackItem*>::Iterator item=items.begin();
    for(;item!=items.end();item++)
    {
        scene->removeItem((*item));
        if((*item)->tracktype==TrackItem::Video)
        {
            videoTrack.removeOne((*item));
        }
        else if((*item)->tracktype==TrackItem::Audio)
        {
            audioTrack.removeOne((*item));
        }
        else if((*item)->tracktype==TrackItem::Caption)
        {
            captionTrack.removeOne((*item));
        }
        else if((*item)->tracktype==TrackItem::Picture)
        {
            pictureTrack.removeOne((*item));
        }
        TrackItem* it=(*item);
        delete it;
    }
    scene->refresh();
}

void TimelineWidget::addVideoTrack(QStandardItem*item)
{
    int dur=item->model()->item(item->row(),1)->data().toInt();
    VideoTrack *v=new VideoTrack(item->text().split(".")[0],dur,scene);
    v->setUrl(item->data().toString());
    v->setImportms(marker->val());
    v->setWithAudio(true);
    scene->addItem(v);
    v->update();
    videoTrack.push_back(v);
    scene->refresh();
}

void TimelineWidget::addAudioTrack(QStandardItem*item)
{
    int dur=item->model()->item(item->row(),1)->data().toInt();
    AudioTrack *v=new AudioTrack(item->text().split(".")[0]+".aac",dur,scene);
    v->setUrl(item->data().toString());
    v->setImportms(marker->val());
    scene->addItem(v);
    audioTrack.push_back(v);
    scene->refresh();
}

void TimelineWidget::addPictureTrack(QStandardItem*item)
{
    PictureTrack *v=new PictureTrack(item->text().split("/").last(),5000,scene,marker->val(),marker->val()+5000);
    v->setUrl(item->data().toString());
    v->pic->setUrl(v->url);
    scene->addItem(v);
    v->update();
    pictureTrack.push_back(v);
    scene->refresh();
    emit newPiconCreated(v->pic);
}

void TimelineWidget::addPiconTrack(Picon*pic)
{
    PictureTrack *v=new PictureTrack(pic->text,5000,scene,marker->val(),marker->val()+5000,pic);
    v->setUrl(pic->url);
    scene->addItem(v);
    pictureTrack.push_back(v);
    v->update();
    scene->refresh();
}

void TimelineWidget::addCaptionTrack(Subtitle*s)
{
    CaptionTrack *v=new CaptionTrack("subtitle",5000,scene,marker->val(),marker->val()+5000,s);
    scene->addItem(v);
    captionTrack.push_back(v);
    scene->refresh();
}

void TimelineWidget::addSRTCaption()
{
    QString url=QFileDialog::getOpenFileName(
                this,
                tr("Select srt file"),
                QDir::currentPath(),
                tr("Caption file (*.srt)"));
    QFile file(url);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray data;
    QString tsp;
    QString text;
    QString next;
    QList<QString> times;
    QString exp;
    int importtime;
    int exporttime;
    int i=0;
    while(!file.atEnd())
    {
        i++;
        data=file.readLine();
        if(file.atEnd())
        {
            break;
        }
        tsp=file.readLine();
        text=file.readLine();
        next=file.readLine();
        while(next!="\r\n")
        {
            text+=next;
            next=file.readLine();
        }
        times=tsp.split(" ");
        importtime=Mstime::hmstoMs(times[0].split(",")[0])+times[0].split(",")[1].toInt();
        exp=times[2].split("\\")[0];
        exporttime=Mstime::hmstoMs(exp.split(",")[0])+exp.split(',')[1].toInt();
        CaptionTrack *ct=new CaptionTrack(text,exporttime-importtime,scene,importtime,exporttime);
        scene->addItem(ct);
        captionTrack.push_back(ct);
        scene->refresh();
    }
    file.close();
}

TimelineWidget::~TimelineWidget()
{
    delete scene;
    delete view;
}
