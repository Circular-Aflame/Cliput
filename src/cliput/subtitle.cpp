#include "subtitle.h"
#include <QApplication>

Subtitle::Subtitle()
{
    setDefaultTextColor(Qt::white);
    lasttext="click to edit";
    setPlainText("click to edit");
}

Subtitle::Subtitle(QGraphicsItem*parent)
    :QGraphicsTextItem(parent)
{
    setDefaultTextColor(Qt::white);
    lasttext="click to edit";
    setPlainText("click to edit");
}

void Subtitle::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
    if(event->button()==Qt::LeftButton)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
    }
    if(isSelected())
    {
        setSelected(false);
    }
    else
    {
        setSelected(true);
    }
    pressed=true;
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    QGraphicsTextItem::mousePressEvent(event);
}

void Subtitle::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
    QPointF newPos=event->scenePos();
    int dx =(newPos-oldMousePos).x();
    int dy=newPos.y()-oldMousePos.y();

    setX(x()+dx);
    setY(y()+dy);

    oldMousePos=event->scenePos();
    oldPos=scenePos();
}

void Subtitle::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
    pressed=false;
    oldMousePos=event->scenePos();
    oldPos=scenePos();
}

void Subtitle::focusInEvent(QFocusEvent*event)
{
    if(event->reason()!=Qt::PopupFocusReason)
    {
        lasttext=toPlainText();
    }
    QGraphicsTextItem::focusInEvent(event);
}

void Subtitle::focusOutEvent(QFocusEvent*event)
{
    //右击外部取消，恢复原文本
    if(event->reason()==Qt::MouseFocusReason&&QApplication::mouseButtons()==Qt::RightButton)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        setPlainText(lasttext);
    }
    else if(event->reason()==Qt::PopupFocusReason)
    {
        //右键菜单可能性
    }
    else
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        emit Editingfinished(true);
    }
    QGraphicsTextItem::focusOutEvent(event);
}
