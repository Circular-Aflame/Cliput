#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include "timelinescene.h"
#include <QGraphicsView>
#include <QWheelEvent>

class TimelineView:public QGraphicsView
{
    Q_OBJECT
public:
    TimelineView(TimelineScene *scene);
    ~TimelineView() {}

protected:
    void wheelEvent(QWheelEvent *event);

private:
    TimelineScene*scene;
};

#endif // TIMELINEVIEW_H
