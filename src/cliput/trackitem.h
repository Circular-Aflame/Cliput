#ifndef TRACKITEM_H
#define TRACKITEM_H

#include "timeline/timelinescene.h"
#include <QGraphicsItem>
#include <QObject>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

class TrackItem:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    enum TrackType {Video=0,
                    Audio,Caption,Picture};
    TrackItem(QString text,int duration,TrackType t,TimelineScene *s);
    TrackItem(QString text,int duration,TrackType t,TimelineScene *s,int importm,int exportm);
    ~TrackItem() {
        delete leftbox;
        delete rightbox;
        scene=nullptr;
    }

    //重载类型以区分
    int type() const override
    {
        return QGraphicsItem::UserType+10;
    }
    //重载
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setTrackType(TrackType t)
    {
        tracktype=t;
    }
    void setHeight(int h)
    {
        height=h;
    }
    void setLength(int l)
    {
        length=l;
    }
    void setUrl(QString filename)
    {
        url=filename;
    }
    void setText(QString t)
    {
        text=t;
    }
    void setExportms(int ms)
    {
        exportms=ms;
        if(exportms>scene->lastms)
        {
            scene->lastms=exportms;
        }
        update();
    }
    void setImportms(int ms)
    {
        importms=ms;
        setPos(importms/step,height*(tracktype+1.5));
        exportms=importms+endms-startms;
        if(exportms>scene->lastms)
        {
            scene->lastms=exportms;
        }
        length=(exportms-importms)/step;
        update();
    }
    void setStart(int ms)
    {
        startms=ms;
    }
    void setEnd(int ms)
    {
        endms=ms;
    }

    //拉伸事件
    QRectF* leftbox;
    QRectF* rightbox;
    int boxwidth;
    bool leftstr;
    bool rightstr;
    int cursortype;

    TrackType tracktype;

    //配色
    QColor col;
    QColor outlineCol;
    QColor selectedColor;
    int rounded;

    QPointF oldPos,oldMousePos;
    TimelineScene* scene;
    bool pressed=false;
    constexpr const static QColor color[4]={QColorConstants::Svg::violet,
                                        QColorConstants::Svg::aquamarine,
                                        QColorConstants::Cyan,
                                        QColorConstants::Svg::palevioletred};

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

public:
    int &step;//dur=step*length
    int length;
    int height;
    int startms;
    int endms;
    int importms;
    int exportms;
    int mos;
    //竖直方向上下界
    int toplim;
    int botlim;
    QString url;
    QString text;
    int markertime;
};

class VideoTrack:public TrackItem
{
    Q_OBJECT
public:
    VideoTrack(QString text,int duration,TimelineScene *s)
        :TrackItem(text,duration,Video,s)
    {
        toplim=height*1.5;
        botlim=height*1.5;
    }
    VideoTrack(QString text,int duration,TimelineScene *s,int imp,int exp)
        :TrackItem(text,duration,Video,s,imp,exp)
    {
        toplim=height*1.5;
        botlim=height*1.5;
    }
    ~VideoTrack() {}
public slots:
    void setWithAudio(bool b)
    {
        withAudio=b;
        if(b)
        {
            text+=".mp4";
        }
        else
        {
            text+=".h264";
        }
    }
    VideoTrack* cut(int cuttime)
    {
        if(cuttime<importms||cuttime>exportms)
        {
            return nullptr;
        }
        VideoTrack* clip=new VideoTrack(text,mos,scene,cuttime,exportms);
        clip->setUrl(url);
        clip->setEnd(endms);
        endms=endms-exportms+cuttime;
        exportms=cuttime;
        clip->setStart(endms);
        return clip;
    }
private:
    bool withAudio;
};

class AudioTrack:public TrackItem
{
    Q_OBJECT
public:
    AudioTrack(QString text,int duration,TimelineScene *s)
        :TrackItem(text,duration,Audio,s)
    {
        toplim=height*2.5;
        botlim=height*2.5;
    }
    AudioTrack(QString text,int duration,TimelineScene *s,int imp,int exp)
        :TrackItem(text,duration,Audio,s,imp,exp)
    {
        toplim=height*2.5;
        botlim=height*2.5;
    }
    ~AudioTrack() {}
public slots:
    AudioTrack* cut(int cuttime)
    {
        if(cuttime<importms||cuttime>exportms)
        {
            return nullptr;
        }
        AudioTrack* clip=new AudioTrack(text,mos,scene,cuttime,exportms);
        clip->setUrl(url);
        clip->setEnd(endms);
        endms=endms-exportms+cuttime;
        exportms=cuttime;
        clip->setStart(endms);
        return clip;
    }
};



#endif // TRACKITEM_H
