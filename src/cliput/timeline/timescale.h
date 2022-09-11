#ifndef TIMESCALE_H
#define TIMESCALE_H

#include "timelinescene.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

class TimeScale:public QGraphicsItem
{
public:
    TimeScale(TimelineScene* s);
    ~TimeScale() {}

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    TimelineScene* scene;
    //刻度像素值
    int largesc;
    int midsc;
    int smallsc;
    int height;
    int rod;
    int length;
    int large_s;
    int mid_s;
    int small_s;
};

#endif // TIMESCALE_H
