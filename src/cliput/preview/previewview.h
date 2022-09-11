#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include "previewvideo.h"
#include "previewscene.h"
#include <QGraphicsView>

class PreviewView:public QGraphicsView
{
    Q_OBJECT
public:
    PreviewView(PreviewScene*s,PreviewVideo*v);
    ~PreviewView() {}
    int wMargin;
    int hMargin;

protected:
    void resizeEvent(QResizeEvent*);

private:
    PreviewScene*scene;
    PreviewVideo*video;
};

#endif // PREVIEWVIEW_H
