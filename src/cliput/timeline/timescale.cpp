#include "timescale.h"
#include "algor.h"
#include <QPainter>

TimeScale::TimeScale(TimelineScene*s)
    :scene(s)
{
    largesc=100;
    midsc=50;
    smallsc=10;
    rod=1;
    height=scene->trackheight*1.5;
    large_s=height*2/5;
    mid_s=height*3/7;
    small_s=height*2/3;
}

QRectF TimeScale::boundingRect() const
{
    return QRectF(0,0,length,height);
}

void TimeScale::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(Qt::black);
    length=scene->lastms/scene->step;
    int x=0;
    while(x<length)
    {
        if(x%largesc==0)
        {
            painter->drawLine(x,large_s,x,height);
            QFont font=scene->font();
            QFontMetrics fontMetrics(font);
            int heightFont=fontMetrics.boundingRect(Mstime::toStamp(x*scene->step)).height();
            painter->drawText(x,heightFont,Mstime::toStamp(x*scene->step));
        }
        else if(x%midsc==0)
        {
            painter->drawLine(x,mid_s,x,height);
        }
        else
        {
            painter->drawLine(x,small_s,x,height);
        }
        x+=smallsc;
    }
}
