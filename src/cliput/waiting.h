#ifndef WAITING_H
#define WAITING_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

class Waiting:public QDialog
{
    Q_OBJECT
public:
    Waiting(QWidget*parent)
        :QDialog(parent)
    {
        //this->setWindowOpacity(0.1);
        setWindowFlag(Qt::FramelessWindowHint);
        resize(QSize(640,360));
        pic=new QPushButton(this);
        pic->resize(QSize(640,360));
        pic->setIcon(QIcon(":/pics/processing.png"));
        pic->setIconSize(QSize(640,360));
    }
    ~Waiting() {}
private:
    QPushButton*pic;
};

#endif // WAITING_H
