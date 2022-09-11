#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include "../timeline/timelineview.h"
#include "../timeline/timelinescene.h"
#include "../trackitem.h"
#include "../captiontrack.h"
#include "../picturetrack.h"
#include "../timeline/timemarker.h"
#include "../timebox.h"
#include "../timeline/timescale.h"
#include <QDockWidget>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QToolBar>
#include <QTimer>

class TimelineWidget:public QDockWidget
{
    Q_OBJECT
public:
    TimelineWidget() {}
    TimelineWidget(QWidget*parent);
    ~TimelineWidget();
    void addview();
    void addToolbar();
public slots:
    void addVideoTrack(QStandardItem*item);
    void addAudioTrack(QStandardItem*item);
    void addPictureTrack(QStandardItem*item);
    void addPiconTrack(Picon*pic);
    void addCaptionTrack(Subtitle*);
    void addSRTCaption();

    //设置缓存路径
    void setCachePath(QString path);

    //特效
    void onAddEffect();
    //导出
    //多视频合并
    void onActionVideoout();
    void onActionCombine();
    void onActionDeleteTrack();
    void onActionCut();
    //片段导出
    void onActionClipout();
    //marker点击移动
    void onMarkerChanged();

    //预览
    void previewPlay(bool isPlaying);
    void onTimeout();
    void previewUpdate();

signals:
    void newClipout(QString filename);
    void tracktoPreview(QList<TrackItem*>&items);
    void previewStop();
    void deleteItems(QList<TrackItem*>&items);
    void newPiconCreated(Picon*);
private:
    QWidget*w;
    QVBoxLayout*layout;
    TimelineView *view;
    TimelineScene*scene;
    TimeMarker* marker;
    QVector<VideoTrack*> videoTrack;
    QVector<AudioTrack*> audioTrack;
    QVector<CaptionTrack*> captionTrack;
    QVector<PictureTrack*> pictureTrack;
    QToolBar *toolbar;
    Timebox *currentBox;
    TimeScale *timescale;
    QString cachePath;
    QTimer* timer;
};

#endif // TIMELINEWIDGET_H
