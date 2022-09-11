#include "trackitem.h"
#include <QStyleOptionGraphicsItem>

TrackItem::TrackItem(QString text,int duration,TrackType t,TimelineScene*s)
    :tracktype(t),scene(s),step(s->step),height(s->trackheight),importms(0),mos(duration),text(text)
{
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);

    col=color[tracktype];
    outlineCol=col.lighter(30);
    selectedColor=QColorConstants::Svg::blueviolet;
    rounded=3;

    height=30;
    startms=0;
    endms=duration;
    exportms=importms+duration;
    setPos(importms/step,height*(tracktype+1.5));

    length=(exportms-importms)/step;
    toplim=height*1.5;
    botlim=height*7.5;
    boxwidth=2;
    leftbox=new QRectF(0,0,boxwidth,height);
    rightbox=new QRectF(endms/step-boxwidth,0,boxwidth,height);
    if(exportms>scene->lastms)
    {
        scene->lastms=exportms;
    }
    leftstr=false;
    rightstr=false;
}

TrackItem::TrackItem(QString text,int duration,TrackType t,TimelineScene *s,int importm,int exportm)
    :tracktype(t),scene(s),step(s->step),height(s->trackheight),importms(importm),exportms(exportm),mos(duration),text(text)
{
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);

    col=color[tracktype];
    outlineCol=col.lighter(30);
    selectedColor=QColorConstants::Svg::blueviolet;
    rounded=3;

    height=30;
    toplim=height*1.5;
    botlim=height*7.5;
    boxwidth=2;
    length=(exportms-importms)/step;
    setPos(importms/step,height*(tracktype+1.5));
    leftbox=new QRectF(0,0,boxwidth,height);
    rightbox=new QRectF(length-boxwidth,0,boxwidth,height);
    if(exportms>scene->lastms)
    {
        scene->lastms=exportms;
    }
    leftstr=false;
    rightstr=false;
}

QRectF TrackItem::boundingRect() const
{
    return QRectF(0,0,(exportms-importms)/step,height);
}

void TrackItem::paint(QPainter*painter,const QStyleOptionGraphicsItem *option,QWidget*widget)
{
    Q_UNUSED(widget);
    //选中效果
    if(option->state&QStyle::State_Selected)
    {
        painter->setBrush(selectedColor);
    }
    else
    {
        painter->setBrush(col);
    }
    length=(exportms-importms)/step;
    setX(importms/step);
    painter->drawRoundedRect(boundingRect(),rounded,rounded);
    painter->setBrush(outlineCol);
    QFont font=scene->font();
    QFontMetrics fontMetrics(font);
    int heightFont=fontMetrics.boundingRect(text).height();
    painter->drawText(5,heightFont,text);

    painter->drawRect(*leftbox);
    //painter->drawRect(*rightbox);
    rightbox->setRect(length-boxwidth,0,boxwidth,height);
}

void TrackItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //判断事件为移动或拉伸
    if(isSelected())
    {
        setSelected(false);
    }
    else
    {
        setSelected(true);
    }
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    if(cursortype==Qt::SizeHorCursor)
    {
        if(leftbox->contains(event->pos()))
        {
            leftstr=true;
            rightstr=false;
        }
        else
        {
            leftstr=false;
            rightstr=true;
        }
    }
    else
    {
        pressed=true;
        this->setCursor(Qt::ClosedHandCursor);
    }
    update();

}

void TrackItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF newPos=event->scenePos();
    int dx =(newPos-oldMousePos).x();
    int dy=newPos.y()-oldPos.y();
    exportms=importms+endms-startms;
    if(pressed)
    {
        //轨道跃迁
        if(abs(dy)>height/2)
        {
            newPos.setY(oldPos.y()+(int)(dy/height)*height);
            if(newPos.y()<=botlim&&newPos.y()>=toplim)
            {
                setY(newPos.y());
            }
        }
        else
        {
            setY(oldPos.y());
        }
        //左侧界限
        importms+=dx*step;
        if(importms<=0)
        {
            importms=0;
        }
        setX(importms/step);
        oldMousePos=event->scenePos();
        oldPos=scenePos();

        //右边界自动扩展
        exportms=importms+endms-startms;
        if(exportms>scene->lastms)
        {
            scene->lastms=exportms;
            scene->refresh();
        }

    }
    else if(leftstr)
    {
        importms+=dx*step;
        startms+=dx*step;
        if(importms<0)
        {
            startms-=importms;
            importms=0;
        }
        if(startms<0)
        {
            importms-=startms;
            startms=0;
        }
        if(endms-startms<1000)
        {
            importms=exportms-1000;
            startms=endms-1000;
        }
        setX(importms/step);
        oldMousePos=event->scenePos();
        oldPos=scenePos();
    }
    else if(rightstr)
    {
        exportms+=dx*step;
        endms+=dx*step;
        if(exportms>scene->lastms)
        {
            scene->lastms=exportms;
            scene->refresh();
        }
        if(endms>mos)
        {
            exportms=exportms-endms+mos;
            endms=mos;
        }
        if(endms-startms<1000)
        {
            exportms=importms+1000;
            endms=startms+1000;
        }
        update();
        oldMousePos=event->scenePos();
        oldPos=scenePos();
    }

}

void TrackItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed=false;
    leftstr=false;
    rightstr=false;
    update();
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    cursortype=Qt::ArrowCursor;
}

void TrackItem::hoverMoveEvent(QGraphicsSceneHoverEvent*event)
{
    //拉伸及移动标识
    QPointF pos=event->pos();
    if(rightbox->contains(pos)||leftbox->contains(pos))
    {
        cursortype=Qt::SizeHorCursor;
        this->setCursor(Qt::SizeHorCursor);
    }
    else
    {
        cursortype=Qt::OpenHandCursor;
        this->setCursor(Qt::OpenHandCursor);
    }
}




