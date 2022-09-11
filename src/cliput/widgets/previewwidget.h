#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "../slider.h"
#include "../subtitle.h"
#include "../picon.h"
#include "../preview/previewscene.h"
#include "../preview/previewview.h"
#include "../preview/previewvideo.h"
#include "../trackitem.h"
#include <QDockWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QTimer>
#include <QSpinBox>
#include <QLabel>
#include <QThread>
#include <QMimeData>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

class PreviewWidget : public QDockWidget
{
    Q_OBJECT
public:
    PreviewWidget() {}
    PreviewWidget(QWidget *parent);
    ~PreviewWidget();
    void addToolBar();
    void addView();
    //允许拖入
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
public slots:
    void onPlay();
    //声音
    void onVol();
    void onVolChanged(int volume);
    //预览
    void trackinPreview(QList<TrackItem*>&items);
    void tracktoDelete(QList<TrackItem*>&items);
    void previewstop();

    void onPiconcreated(Picon*);

signals:
    void isplaying(bool);
    void newSubcreated(Subtitle*);
    void newPiconcreated(Picon*);

private:
    QWidget*w;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QVBoxLayout* layout;

    PreviewView* view;
    PreviewScene* scene;
    PreviewVideo* videoWidget;

    QAction* actResume;
    QAction* actPause;
    QAction* actVol;
    QAction* actMuted;
    Slider* volSlider;
    bool playing;
    qint64 posTime;
    bool posUndo;

    QRegularExpression*rx;
    QRegularExpressionValidator* val;
};

#endif // PREVIEWWIDGET_H
