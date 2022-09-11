#ifndef ALGOR_H
#define ALGOR_H

#include <QString>

class Mstime
{
public:
    Mstime() {}
    ~Mstime() {}
    static QString toHMS(int ms);
    static QString toHMSM(int ms);
    static QString toStamp(int ms);
    static int hmstoMs(QString hms);
    static int hmsmtoMs(QString hmsm);
};

#endif // ALGOR_H
