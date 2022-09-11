#include "timelinescene.h"
#include "trackitem.h"
#include <QGraphicsItem>

TimelineScene::TimelineScene(QObject*parent)
    :QGraphicsScene(parent)
{
    ind=2;
    step=steps[ind];
    trackheight=30;
    lastms=240000;
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

QList<TrackItem*> TimelineScene::allTracks()
{
    return filterTracks(this->items());
}

//过滤获得轨道
QList<TrackItem*> TimelineScene::filterTracks(const QList<QGraphicsItem*>&items)
{
    QList<TrackItem*> tracks;
    //单调输出
    for(auto item:items)
    {
        if(item->type()==QGraphicsItem::UserType+10)
        {
            TrackItem *it=(TrackItem*)item;
            if(tracks.isEmpty())
            {
                tracks.append(it);
            }
            else
            {
                bool insert=false;
                QList<TrackItem*>::Iterator i=tracks.begin();
                for(;i!=tracks.end();i++)
                {
                    if((*i)->importms>it->importms)
                    {
                        tracks.insert(i,it);
                        insert=true;
                        break;
                    }
                }
                if(!insert)
                {
                    tracks.append(it);
                }
            }
        }
    }
    return tracks;
}

QList<TrackItem*> TimelineScene::selectedTracks()
{
    return filterTracks(selectedItems());
}


void TimelineScene::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
    QGraphicsScene::mousePressEvent(event);
    if(event->scenePos().y()<=trackheight*1.5)
    {
        emit markerChanged(event->scenePos().x()*step);
    }
}

void TimelineScene::refresh()
{
    this->setSceneRect(0,0,lastms/step,8.5*trackheight);
    update();
    emit review();
}

void TimelineScene::setStepIndex(int index)
{
    ind-=index;
    if(ind<=9&&ind>=0)
    {
        step=steps[ind];
    }
    else if(ind>9)
    {
        step=steps[9];
        ind=9;
    }
    else
    {
        step=steps[0];
        ind=0;
    }
}
