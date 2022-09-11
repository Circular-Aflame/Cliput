#include "timemarker.h"
#include <QPainter>

TimeMarker::TimeMarker(TimelineScene*s)
    :scene(s),step(s->step)
{
    value=6000;
    height=s->height();
    start=s->trackheight/2;
    hori=8;
    vert=start;
    setPos(value/step-hori/2,start);
    setZValue(100);
    interval=10;
}

void TimeMarker::setValue(int v)
{
    value=v;
    setX(value/step-hori/2);
    emit valueChanged(v);
}

QRectF TimeMarker::boundingRect() const
{
    return QRect(0,0,hori,height-start);
}

void TimeMarker::paint(QPainter*painter,const QStyleOptionGraphicsItem *option,QWidget*widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    setPos(value/step-hori/2,start);
    painter->setPen(QColorConstants::DarkBlue);
    painter->drawLine(hori/2,vert,hori/2,height);

    QPolygonF head;
    head<<QPointF(0,0)<<QPointF(0,vert-5)
       <<QPointF(hori/2,vert)<<QPointF(hori,vert-5)
       <<QPointF(hori,0);
    painter->setBrush(QColorConstants::LightGray);
    painter->setPen(QColorConstants::DarkGray);
    painter->drawPolygon(head);
}

void TimeMarker::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
    oldMousePos=event->scenePos();
    oldPos=scenePos();
}

void TimeMarker::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
    QPointF newPos=event->scenePos();
    int dx =(newPos-oldMousePos).x();
    value+=dx*step;
    if(value<0)
    {
        value=0;
    }
    setX(value/step-hori/2);
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    emit valueChanged(value);
    emit positionMoved(value);
}

void TimeMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
    oldMousePos=event->scenePos();
    oldPos=scenePos();
}







