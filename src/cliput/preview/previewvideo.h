#ifndef PREVIEWVIDEO_H
#define PREVIEWVIDEO_H

#include <QGraphicsVideoItem>

class PreviewVideo:public QGraphicsVideoItem
{
    Q_OBJECT
public:
    PreviewVideo()
    {
        setAspectRatioMode(Qt::KeepAspectRatio);
    }
    ~PreviewVideo() {}
    QRectF boundingRect()const override
    {
        return QRectF(0,0,QGraphicsVideoItem::boundingRect().width(),QGraphicsVideoItem::boundingRect().height());
    }
};

#endif // PREVIEWVIDEO_H
