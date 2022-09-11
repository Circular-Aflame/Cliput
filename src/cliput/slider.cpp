#include "slider.h"

Slider::Slider(QWidget*parent):QSlider(Qt::Horizontal,parent)
{

}

void Slider::mousePressEvent(QMouseEvent *event)
{
    //优先进行原处理
    QSlider::mousePressEvent(event);
    int currentX=event->pos().x();
    int val=0.5+currentX*this->maximum()/this->width();
    this->setValue(val);
    emit probarClicked();
}

Slider::~Slider()
{

}
