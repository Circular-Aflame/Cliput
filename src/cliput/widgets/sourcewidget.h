#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include "../slider.h"
#include "../timebox.h"
#include <QDockWidget>
#include <QEvent>
#include <QApplication>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QSlider>
#include <QTimer>
#include <QSpinBox>
#include <QLabel>

class SourceWidget : public QDockWidget
{
    Q_OBJECT
public:
    SourceWidget() {}
    SourceWidget(QWidget* parent);
    ~SourceWidget();
    void addToolBar();
    void addProgressBar();
public slots:
    void addVideo(QString filename);
    void play();
    void pause();
    void onPlay();
    //播放状态改变事件，停止后暂停
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState);
    //声音
    void onVol();
    void onVolChanged(int volume);
    //进度条
    void setPosOnClicked();
    void setPosOnMoved();
    void onReleased();
    void onTimeout();
    void onBox();

private:
    QWidget* w;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    QVideoWidget* videoWidget;
    QVBoxLayout* layout;

    QAction* actResume;
    QAction* actPause;
    QAction* actVol;
    QAction* actMuted;
    Slider* volSlider;
    Slider* progressbar;
    QTimer* timer;
    Timebox* probox;
    QLabel* total;
};

#endif //SOURCEWIDGET_H
