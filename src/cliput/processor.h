#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QProcess>
#include <QDebug>

//便捷命令符
class Processor:public QProcess
{
    Q_OBJECT
public:
    Processor(QObject*parent):QProcess(parent)
    {
        this->setProcessChannelMode(QProcess::MergedChannels);
    }
    ~Processor() {}

public slots:
    bool process(QStringList arguments)
    {
        this->start("ffmpeg",arguments);
        this->waitForStarted();
        return waitForFinished(60000);

    }
};

#endif // PROCESSOR_H
