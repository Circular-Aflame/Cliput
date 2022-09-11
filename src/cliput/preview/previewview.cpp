#include "previewview.h"

PreviewView::PreviewView(PreviewScene*s,PreviewVideo*v)
    :scene(s),video(v)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(FullViewportUpdate);
    setMouseTracking(true);
    setAlignment(Qt::AlignLeft);
    setAcceptDrops(false);
}

void PreviewView::resizeEvent(QResizeEvent*)
{
    //使播放窗口居中
    scene->setSceneRect(0,0,width(),height());
    video->setPos(0,0);
    if(width()*9>height()*16)
    {
        video->setSize(QSizeF(height()*16/9,height()));
        wMargin=(width()-height()*16/9)/2;
        hMargin=0;
        video->setX(wMargin);
    }
    else
    {
        video->setSize(QSizeF(width(),width()*9/16));
        hMargin=(height()-width()*9/16)/2;
        wMargin=0;
        video->setY(hMargin);
    }
}












