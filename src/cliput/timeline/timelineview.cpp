#include "timelineview.h"
#include <QApplication>

TimelineView::TimelineView(TimelineScene*s)
    :QGraphicsView(s)
{
    scene=s;
    this->setAlignment(Qt::AlignLeft);
    this->setMouseTracking(true);
    this->setViewportUpdateMode(FullViewportUpdate);
}

void TimelineView::wheelEvent(QWheelEvent*event)
{
    //ctrl+wheel控制水平刻度缩放
    if(QApplication::keyboardModifiers()==Qt::ControlModifier)
    {
        int rate =event->angleDelta().y()/120;
        scene->setStepIndex(rate);
        scene->refresh();
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}
