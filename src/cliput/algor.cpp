#include "algor.h"
#include <QList>

QString Mstime::toHMS(int ms)
{
    int sec=ms/1000;
    int min=(sec-sec%60)/60;
    int hour=(min-min%60)/60;
    //补足两位
    return QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(min%60,2,10,QChar('0')).arg(sec%60,2,10,QChar('0'));
}

QString Mstime::toStamp(int ms)
{
    int sec=ms/1000;
    int min=(sec-sec%60)/60;
    int hour=(min-min%60)/60;
    //补足两位
    if(hour==0)
    {
        return QString("%2:%3").arg(min%60,2,10,QChar('0')).arg(sec%60,2,10,QChar('0'));
    }
    else
    {
        return QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(min%60,2,10,QChar('0')).arg(sec%60,2,10,QChar('0'));
    }

}

QString Mstime::toHMSM(int ms)
{
    int sec=ms/1000;
    int min=(sec-sec%60)/60;
    int hour=(min-min%60)/60;
    //补足两位
    return QString("%1:%2:%3.%4").arg(hour,2,10,QChar('0')).arg(min%60,2,10,QChar('0')).arg(sec%60,2,10,QChar('0')).arg(ms%1000,3,10,QChar('0'));
}

int Mstime::hmstoMs(QString hms)
{
    QList<QString> num=hms.split(":");
    return 1000*(num[0].toInt()*3600+num[1].toInt()*60+num[2].toInt());
}

int Mstime::hmsmtoMs(QString hmsm)
{
    QList<QString> num=hmsm.split(":");
    QList<QString> sms=num[2].split(".");
    return 1000*(num[0].toInt()*3600+num[1].toInt()*60+sms[0].toInt())+sms[1].toInt();
}
