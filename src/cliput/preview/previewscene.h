#ifndef PREVIEWSCENE_H
#define PREVIEWSCENE_H

#include "subtitle.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PreviewScene:public QGraphicsScene
{
    Q_OBJECT
public:
    PreviewScene(QObject*parent=nullptr);
    ~PreviewScene() {}

signals:
    void newSubcreated(Subtitle*sub);

protected:
    //双击添加字幕
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event);
};

#endif // PREVIEWSCENE_H
