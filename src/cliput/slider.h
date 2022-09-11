#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QMouseEvent>

class Slider:public QSlider
{
    Q_OBJECT
public:
    Slider(QWidget*parent=nullptr);
    ~Slider();
signals:
    void probarClicked();

protected:
    //实现鼠标点击后滑块跟随
    void mousePressEvent(QMouseEvent *event);

};

#endif // SLIDER_H
