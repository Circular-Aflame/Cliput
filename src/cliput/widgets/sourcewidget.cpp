#include "sourcewidget.h"
#include "../algor.h"
#include <QToolBar>
#include <QStyle>
#include <QLabel>

SourceWidget::SourceWidget(QWidget* parent)
    :QDockWidget(parent)
{
    setWindowTitle("Source");
    //主窗口
    w = new QWidget(this);
    this->setWidget(w);
    //初始化播放器
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    videoWidget = new QVideoWidget(this);
    player->setVideoOutput(videoWidget);
    videoWidget->resize(QSize(800, 450));

    layout = new QVBoxLayout(w);
    layout->addWidget(videoWidget);

    addProgressBar();
    addToolBar();
}

void SourceWidget::addProgressBar()
{
    progressbar = new Slider(this);
    layout->addWidget(progressbar);
    progressbar->setMinimum(0);
    progressbar->setDisabled(true);
    timer = new QTimer(this);
    //进度控制
    connect(progressbar, SIGNAL(probarClicked()), this, SLOT(setPosOnClicked()));
    connect(progressbar, SIGNAL(sliderMoved(int)), this, SLOT(setPosOnMoved()));
    connect(progressbar, SIGNAL(sliderReleased()), this, SLOT(onReleased()));
    //数值控制
    probox = new Timebox(this);
    probox->setMinimum(0);
    probox->setDisabled(true);
    probox->setFrame(false);
    connect(progressbar, SIGNAL(valueChanged(int)), probox, SLOT(setValue(int)));
    connect(probox, SIGNAL(editingFinished()), this, SLOT(onBox()));
}

void SourceWidget::onBox()
{
    progressbar->setValue(probox->value());
    emit progressbar->probarClicked();
}

void SourceWidget::onTimeout()
{
    progressbar->setValue(static_cast<qint64>(player->position()));
}

void SourceWidget::onReleased()
{
    timer->start();
}

void SourceWidget::setPosOnMoved()
{
    timer->stop();
    player->setPosition(static_cast<int>(progressbar->value()));
}

void SourceWidget::setPosOnClicked()
{
    player->setPosition(static_cast<int>(progressbar->value()));
}

void SourceWidget::addToolBar()
{
    QToolBar* toolbar = new QToolBar(this);
    layout->addWidget(toolbar, Qt::BottomToolBarArea);
    //probox
    toolbar->addWidget(probox);
    //Resume
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MediaPlay);
    actResume = toolbar->addAction(icon, "Resume", this, &SourceWidget::onPlay);
    actResume->setDisabled(true);
    icon = QApplication::style()->standardIcon(QStyle::SP_MediaPause);
    actPause = toolbar->addAction(icon, "Pause", this, &SourceWidget::onPlay);
    actPause->setVisible(false);
    connect(player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)), this, SLOT(onPlaybackStateChanged(QMediaPlayer::PlaybackState)));

    //Volume
    icon = QApplication::style()->standardIcon(QStyle::SP_MediaVolume);
    actVol = toolbar->addAction(icon, "Volume", this, &SourceWidget::onVol);
    icon = QApplication::style()->standardIcon(QStyle::SP_MediaVolumeMuted);
    actMuted = toolbar->addAction(icon, "Muted", this, &SourceWidget::onVol);
    actMuted->setVisible(false);

    volSlider = new Slider(this);
    volSlider->setMinimum(0);
    volSlider->setMaximum(100);
    volSlider->setValue(100);

    toolbar->addWidget(volSlider);
    connect(volSlider, SIGNAL(valueChanged(int)), this, SLOT(onVolChanged(int)));

    //总时长
    total = new QLabel(this);
    total->setText("--:--:--");
    total->setDisabled(true);
    toolbar->addWidget(total);
}

void SourceWidget::onVolChanged(int volume)
{
    audioOutput->setVolume(volume / 100.0);
}

void SourceWidget::onVol()
{
    if (audioOutput->isMuted())
    {
        actMuted->setVisible(false);
        actVol->setVisible(true);
        volSlider->setEnabled(true);
        audioOutput->setMuted(false);
    }
    else
    {
        actMuted->setVisible(true);
        actVol->setVisible(false);
        volSlider->setDisabled(true);
        audioOutput->setMuted(true);
    }
}

void SourceWidget::onPlay()
{
    switch (player->playbackState())
    {
    case QMediaPlayer::PlayingState:
        player->pause();
        actResume->setVisible(true);
        actPause->setVisible(false);
        break;
    case QMediaPlayer::StoppedState:
        if (!player->hasVideo())
        {
            break;
        }
    case QMediaPlayer::PausedState:
        player->play();
        actResume->setVisible(false);
        actPause->setVisible(true);
        break;
    }
}

void SourceWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState)
{
    switch (player->playbackState())
    {
    case QMediaPlayer::PlayingState:
        if (progressbar->value() == 0)
        {
            //调整进度条
            progressbar->setMaximum(static_cast<qint64>(player->duration()));
            probox->setMaximum(progressbar->maximum());
            progressbar->setEnabled(true);
            progressbar->setValue(0);
            probox->setEnabled(true);
            probox->setValue(0);
            timer->setInterval(100);
            timer->start();
            connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
            //总时长显示
            total->setEnabled(true);
            total->setText(Mstime::toHMS(player->duration()));
        }
        break;
    case QMediaPlayer::StoppedState:
        if (!player->hasVideo())
        {
            break;
        }
        else
        {
            actResume->setVisible(true);
            actPause->setVisible(false);
            break;
        }
    case QMediaPlayer::PausedState:
        break;
    }
}

void SourceWidget::addVideo(QString filename)
{
    player->setSource(filename);
    actResume->setEnabled(true);
}

void SourceWidget::play()
{
    player->play();
}

void SourceWidget::pause()
{
    player->pause();
}

SourceWidget::~SourceWidget()
{
}
