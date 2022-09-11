#include "previewwidget.h"
#include "../captiontrack.h"
#include "../picturetrack.h"
#include <QToolBar>
#include <QStyle>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QDragEnterEvent>
#include <QDropEvent>

PreviewWidget::PreviewWidget(QWidget *parent)
    :QDockWidget(parent)
{
    setWindowTitle("Preview");
    //主窗口
    w=new QWidget(this);
    this->setWidget(w);
    //初始化播放器
    player=new QMediaPlayer(this);
    audioOutput=new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    videoWidget=new PreviewVideo;
    player->setVideoOutput(videoWidget);
    playing=false;

    layout=new QVBoxLayout(w);
    addView();
    addToolBar();
    rx=new QRegularExpression(".*\\.(jpg|png|)");
    val=new QRegularExpressionValidator(*rx,0);
    setAcceptDrops(true);
}

void PreviewWidget::addView()
{
    scene=new PreviewScene();
    view=new PreviewView(scene,videoWidget);
    view->setScene(scene);
    layout->addWidget(view);
    scene->addItem(videoWidget);
    connect(scene,SIGNAL(newSubcreated(Subtitle*)),this,SIGNAL(newSubcreated(Subtitle*)));
}


void PreviewWidget::dragEnterEvent(QDragEnterEvent*event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    int pos=0;
    QString s;
    for(auto url:urls)
    {
        s=url.toLocalFile();
        if(val->validate(s,pos)==QValidator::Acceptable)
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

void PreviewWidget::dropEvent(QDropEvent*event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    int pos=0;
    QString s;
    for(auto url:urls)
    {
        s=url.toLocalFile();
        if(val->validate(s,pos)==QValidator::Acceptable)
        {
            Picon*pic=new Picon();
            pic->url=s;
            pic->setUrl(s);
            pic->text=s.split("/").last();
            pic->setPos(event->position());
            scene->addItem(pic);
            emit newPiconcreated(pic);
        }
    }
}

void PreviewWidget::onPiconcreated(Picon* pic)
{
    scene->addItem(pic);
}

void PreviewWidget::addToolBar()
{
    QToolBar* toolbar=new QToolBar(this);
    layout->addWidget(toolbar,Qt::BottomToolBarArea);
    //Resume
    QIcon icon=QApplication::style()->standardIcon(QStyle::SP_MediaPlay);
    actResume=toolbar->addAction(icon,"Resume",this,&PreviewWidget::onPlay);
    icon=QApplication::style()->standardIcon(QStyle::SP_MediaPause);
    actPause=toolbar->addAction(icon,"Pause",this,&PreviewWidget::onPlay);
    actPause->setVisible(false);

    //Volume
    icon=QApplication::style()->standardIcon(QStyle::SP_MediaVolume);
    actVol=toolbar->addAction(icon,"Volume",this,&PreviewWidget::onVol);
    icon=QApplication::style()->standardIcon(QStyle::SP_MediaVolumeMuted);
    actMuted=toolbar->addAction(icon,"Muted",this,&PreviewWidget::onVol);
    actMuted->setVisible(false);

    volSlider=new Slider(this);
    volSlider->setMinimum(0);
    volSlider->setMaximum(100);
    volSlider->setValue(100);

    toolbar->addWidget(volSlider);
    connect(volSlider,SIGNAL(valueChanged(int)),this,SLOT(onVolChanged(int)));
}

void PreviewWidget::onVolChanged(int volume)
{
    audioOutput->setVolume(volume/100.0);
}

void PreviewWidget::onVol()
{
    if(audioOutput->isMuted())
    {
        actMuted->setVisible(false);
        actVol->setVisible(true);
        volSlider->setEnabled(true);
        audioOutput->setMuted(false);
    }
    else
    {
        actMuted->setVisible(true);
        actVol->setVisible(false);
        volSlider->setDisabled(true);
        audioOutput->setMuted(true);
    }
}

void PreviewWidget::onPlay()
{
    switch(player->playbackState())
    {
    case QMediaPlayer::PlayingState:
        playing=false;
        emit isplaying(false);

        player->pause();
        actResume->setVisible(true);
        actPause->setVisible(false);
        break;
    case QMediaPlayer::StoppedState:
        if(actResume->isVisible())
        {
            playing=true;
            emit isplaying(true);
            actResume->setVisible(false);
            actPause->setVisible(true);
            break;
        }
        else
        {
            playing=false;
            emit isplaying(false);
            actResume->setVisible(true);
            actPause->setVisible(false);
            break;
        }
    case QMediaPlayer::PausedState:
        playing=true;
        emit isplaying(true);

        actResume->setVisible(false);
        actPause->setVisible(true);
        break;
    }
}

void PreviewWidget::previewstop()
{
    player->pause();
    playing=false;
    emit isplaying(false);
    actResume->setVisible(true);
    actPause->setVisible(false);
}

void PreviewWidget::tracktoDelete(QList<TrackItem*>&items)
{
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Caption)
        {
            CaptionTrack *it=(CaptionTrack*)item;
            if(it->sub->scene()==scene)
            {
                scene->removeItem(it->sub);
            }
        }
        else if(item->tracktype==TrackItem::Picture)
        {
            PictureTrack *it=(PictureTrack*)item;
            if(it->pic->scene()==scene)
            {
                scene->removeItem(it->pic);
            }
        }
    }
}

void PreviewWidget::trackinPreview(QList<TrackItem*>&items)
{
    //黑幕部分
    bool videoIn=false;
    for(auto item:items)
    {
        if(item->tracktype==TrackItem::Video)
        {
            if(videoIn)
            {
                continue;
            }
            videoIn=true;
            VideoTrack *it=(VideoTrack*)item;
            if(player->source()!=item->url)
            {
                player->setSource(item->url);
                posTime=static_cast<int>(it->markertime-it->importms+it->startms);
                player->setPosition(posTime);
                posUndo=true;
            }
            if(playing)
            {
                if(player->playbackState()!=QMediaPlayer::PlayingState)
                {
                    player->setPosition(static_cast<int>(it->markertime-it->importms+it->startms));
                    player->play();
                }
                if(posUndo)
                {
                    player->setPosition(static_cast<int>(it->markertime-it->importms+it->startms));
                    if(player->position()!=static_cast<int>(it->markertime-it->importms+it->startms))
                    {
                        posUndo=true;
                    }
                    else
                    {
                        posUndo=false;
                        player->pause();
                        emit isplaying(false);
                    }
                    player->setPosition(static_cast<int>(it->markertime-it->importms+it->startms));
                    if(player->playbackState()==QMediaPlayer::PausedState)
                    {
                        onPlay();
                    }
                }
            }
            else
            {
                player->setPosition(static_cast<int>(it->markertime-it->importms+it->startms));
            }
        }
        else if(item->tracktype==TrackItem::Audio)
        {
            //AudioTrack *it=(AudioTrack*)item;
        }
        else if(item->tracktype==TrackItem::Caption)
        {
            CaptionTrack *it=(CaptionTrack*)item;
            if(it->sub->scene()!=scene)
            {
                scene->addItem(it->sub);
            }
            if(it->subti)
            {
                it->sub->setPos((view->width()-it->sub->boundingRect().width())/2,view->height()-view->hMargin-it->sub->boundingRect().height());
            }
        }
        else if(item->tracktype==TrackItem::Picture)
        {
            PictureTrack *it=(PictureTrack*)item;
            if(it->pic->scene()!=scene)
            {
                scene->addItem(it->pic);
            }
            //渐入渐出
            if(it->fadein&&(it->markertime-it->importms<=it->fadeInTill))
            {
                it->pic->setOpacity(1.0*(it->markertime-it->importms)/it->fadeInTill);
            }
            else if(it->fadeout&&(it->exportms-it->markertime<=it->fadeOutFrom))
            {
                it->pic->setOpacity(1.0*(it->exportms-it->markertime)/it->fadeOutFrom);
            }
            else
            {
                it->pic->setOpacity(1);
            }
            //平移
            double k=1.0*(it->markertime-it->importms)/(it->exportms-it->importms);
            if(it->translation)
            {
                it->pic->setX(view->wMargin+it->transtart.x()+k*(it->transend.x()-it->transtart.x()));
                it->pic->setY(view->hMargin+it->transtart.y()+k*(it->transend.y()-it->transtart.y()));
            }
            if(it->scaling)
            {
                it->pic->width=it->scalstart.width()+k*(it->scalend.width()-it->scalstart.width());
                it->pic->height=it->scalstart.height()+k*(it->scalend.height()-it->scalstart.height());
            }
        }
    }
    if(!videoIn)
    {
        player->stop();
        player->setSource(QString());
    }
}

PreviewWidget::~PreviewWidget()
{
}
