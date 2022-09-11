#ifndef TIMEMARKER_H
#define TIMEMARKER_H

#include "timelinescene.h"
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class TimeMarker:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    TimeMarker(TimelineScene*s);
    ~TimeMarker() {}

    int interval;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter*painter,const QStyleOptionGraphicsItem *option,QWidget*widget) override;

signals:
    void valueChanged(int v);
    void positionMoved(int v);

public slots:
    void setValue(int v);
    int val() const
    {
        return value;
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    TimelineScene*scene;
    QPointF oldPos,oldMousePos;
    int value;
    int height;
    int start;
    //头部
    int vert;
    int hori;
    int&step;
};

#endif // TIMEMARKER_H
