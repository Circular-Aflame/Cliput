#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include <QImage>

class Model:public QStandardItemModel
{
    Q_OBJECT
public:
    Model(QObject*parent);
    ~Model() {}
    int countVideo;
    int countPicture;

public slots:
    //获取缩略图等
    void addVideo(int arow,QStandardItem*item);
    QImage thumbnail(QString url);
    void setThumbPersent(int);
    void onrenew();

private:
    float thumbPercent;
    int duration;
};

#endif // MODEL_H
