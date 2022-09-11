#include "previewscene.h"

PreviewScene::PreviewScene(QObject*parent)
    :QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(Qt::black);
}

//双击加字幕
void PreviewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    Subtitle* sub=new Subtitle;
    addItem(sub);
    sub->setPos(event->scenePos());
    emit newSubcreated(sub);
}


