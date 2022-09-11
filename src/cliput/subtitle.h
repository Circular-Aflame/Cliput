#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>

class Subtitle:public QGraphicsTextItem
{
    Q_OBJECT
public:
    Subtitle();
    Subtitle(QGraphicsItem*parent);
    ~Subtitle(){}
    QString lasttext;
    bool pressed;
    QPointF oldMousePos,oldPos;


signals:
    void Editingfinished(bool);

protected:
    //编辑移动字幕
    void focusInEvent(QFocusEvent*event) override;
    void focusOutEvent(QFocusEvent*event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // SUBTITLE_H
