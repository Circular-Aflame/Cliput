#include "picon.h"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QPainter>

Picon::Picon()
{
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setAcceptHoverEvents(true);

    width=320;
    height=180;
    box=4;

    topbox=new QRectF(box,0,width-2*box,box);
    botbox=new QRectF(box,height-box,width-2*box,box);
    leftbox=new QRectF(0,box,box,height-2*box);
    rightbox=new QRectF(width-box,box,box,height-2*box);

    ltb=new QRectF(0,0,box,box);
    rtb=new QRectF(width-box,0,box,box);
    lbb=new QRectF(0,height-box,box,box);
    rbb=new QRectF(width-box,height-box,box,box);

    left=false;
    right=false;
    top=false;
    bot=false;
    lt=rt=lb=rb=false;
}

QRectF Picon::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void Picon::setUrl(QString u)
{
    url=u;
    map=new QPixmap(QPixmap::fromImage(QImage(url)));
    width=ow=map->width()/10;
    ow/=2;
    height=oh=map->height()/10;
    oh/=2;
}

void Picon::paint(QPainter*painter,const QStyleOptionGraphicsItem *option,QWidget*widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPixmap(boundingRect(),*map,map->rect());
    if(isSelected())
    {
        painter->setPen(QPen(QColorConstants::Green, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawLine(0,0,width,0);
        painter->drawLine(0,0,0,height);
        painter->drawLine(0,height,width,height);
        painter->drawLine(width,0,width,height);
    }
    topbox->setRect(box,0,width-2*box,box);
    botbox->setRect(box,height-box,width-2*box,box);
    leftbox->setRect(0,box,box,height-2*box);
    rightbox->setRect(width-box,box,box,height-2*box);
    ltb->setRect(0,0,box,box);
    rtb->setRect(width-box,0,box,box);
    lbb->setRect(0,height-box,box,box);
    rbb->setRect(width-box,height-box,box,box);
}

void Picon::mousePressEvent(QGraphicsSceneMouseEvent*event)
{
    //判断事件为移动或拉伸
    if(isSelected())
    {
        setSelected(false);
    }
    else
    {
        setSelected(true);
    }
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    if(cursortype==Qt::SizeHorCursor)
    {
        if(leftbox->contains(event->pos()))
        {
            left=true;
            right=false;
        }
        else
        {
            left=false;
            right=true;
        }
    }
    else if(cursortype==Qt::SizeVerCursor)
    {
        if(topbox->contains(event->pos()))
        {
            top=true;
            bot=false;
        }
        else
        {
            top=false;
            bot=true;
        }
    }
    else if(cursortype==Qt::SizeBDiagCursor)
    {
        if(rtb->contains(event->pos()))
        {
            rt=true;
            lb=false;
        }
        else
        {
            rt=false;
            lb=true;
        }
    }
    else if(cursortype==Qt::SizeFDiagCursor)
    {
        if(ltb->contains(event->pos()))
        {
            lt=true;
            rb=false;
        }
        else
        {
            lt=false;
            rb=true;
        }
    }
    else
    {
        pressed=true;
        this->setCursor(Qt::ClosedHandCursor);
    }
}

void Picon::mouseMoveEvent(QGraphicsSceneMouseEvent*event)
{
    QPointF newPos=event->scenePos();
    int dx =(newPos-oldMousePos).x();
    int dy=newPos.y()-oldMousePos.y();
    if(pressed)
    {
        setX(x()+dx);
        setY(y()+dy);
    }
    else if(left)
    {
        if(width-dx>=ow)
        {
            width-=dx;
        }
        else
        {
            dx=width-ow;
            width=ow;
        }
        setX(oldPos.x()+dx);
    }
    else if(right)
    {
        if(width+dx>=ow)
        {
            width+=dx;
        }
        else
        {
            width=ow;
        }
    }
    else if(top)
    {
        if(height-dy>=oh)
        {
            height-=dy;
        }
        else
        {
            dy=height-oh;
            height=oh;
        }
        setY(oldPos.y()+dy);
    }
    else if(bot)
    {
        if(height+dy>=oh)
        {
            height+=dy;
        }
        else
        {
            dy=height+oh;
            height=oh;
        }
    }
    else if(rt)
    {
        if(abs(dx)*oh>abs(dy)*ow)
        {
            width+=dx;
            dy=height-width*oh/ow;
            height=width*oh/ow;
        }
        else
        {
            height-=dy;
            width=height*ow/oh;
        }
        setY(y()+dy);
    }
    else if(lb)
    {
        if(abs(dx)*oh>abs(dy)*ow)
        {
            width-=dx;
            height=width*oh/ow;
        }
        else
        {
            height+=dy;
            dx=width-height*ow/oh;
            width=height*ow/oh;
        }
        setX(x()+dx);
    }
    else if(lt)
    {
        if(abs(dx)*oh>abs(dy)*ow)
        {
            width-=dx;
            dy=height-width*oh/ow;
            height=width*oh/ow;
        }
        else
        {
            height-=dy;
            dx=width-height*ow/oh;
            width=height*ow/oh;
        }
        setX(x()+dx);
        setY(y()+dy);
    }
    else if(rb)
    {
        if(abs(dx)*oh>abs(dy)*ow)
        {
            width+=dx;
            height=width*oh/ow;
        }
        else
        {
            height+=dy;
            width=height*ow/oh;
        }
    }
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    update();
}

void Picon::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
    pressed=false;
    left=false;
    right=false;
    top=false;
    bot=false;
    lt=lb=rt=rb=false;
    update();
    oldMousePos=event->scenePos();
    oldPos=scenePos();
    cursortype=Qt::ArrowCursor;
    this->setCursor(Qt::ArrowCursor);
}

void Picon::hoverMoveEvent(QGraphicsSceneHoverEvent*event)
{
    QPointF pos=event->pos();
    if(rightbox->contains(pos)||leftbox->contains(pos))
    {
        cursortype=Qt::SizeHorCursor;
        this->setCursor(Qt::SizeHorCursor);
    }
    else if(topbox->contains(pos)||botbox->contains(pos))
    {
        cursortype=Qt::SizeVerCursor;
        this->setCursor(Qt::SizeVerCursor);
    }
    else if(rtb->contains(pos)||lbb->contains(pos))
    {
        cursortype=Qt::SizeBDiagCursor;
        this->setCursor(Qt::SizeBDiagCursor);
    }
    else if(rbb->contains(pos)||ltb->contains(pos))
    {
        cursortype=Qt::SizeFDiagCursor;
        this->setCursor(Qt::SizeFDiagCursor);
    }
    else
    {
        cursortype=Qt::OpenHandCursor;
        this->setCursor(Qt::OpenHandCursor);
    }
}









