#ifndef PICTURETRACK_H
#define PICTURETRACK_H

#include "trackitem.h"
#include "picon.h"

class PictureTrack:public TrackItem
{
    Q_OBJECT
public:
    PictureTrack(QString text,int duration,TimelineScene *s,int importm,int exportm)
        :TrackItem(text,duration,Picture,s,importm,exportm)
    {
        startms=importms;
        endms=exportms;
        toplim=height*4.5;
        botlim=height*7.5;
        mos=1000000;
        pic=new Picon();
        pic->text=text;
        fadein=fadeout=translation=scaling=false;
    }
    PictureTrack(QString text,int duration,TimelineScene *s,int importm,int exportm,Picon*p)
        :TrackItem(text,duration,Picture,s,importm,exportm)
    {
        startms=importms;
        endms=exportms;
        toplim=height*3.5;
        botlim=height*4.5;
        mos=1000000;
        pic=p;
        fadein=fadeout=translation=scaling=false;
    }

    ~PictureTrack()
    {
        delete pic;
    }
    Picon* pic;
    int fadeInTill;
    int fadeOutFrom;
    bool fadein,fadeout,translation,scaling;
    QPointF transtart,transend;
    QRectF scalstart,scalend;

public slots:
    void setscalendh(int w)
    {
        scalend.setHeight(w);
    }
    void setscalendw(int w)
    {
        scalend.setWidth(w);
    }
    void setscalstarth(int w)
    {
        scalstart.setHeight(w);
    }
    void setscalstartw(int w)
    {
        scalstart.setWidth(w);
    }
    void setTransendy(int y)
    {
        transend.setY(y);
    }
    void setTransendx(int x)
    {
        transend.setX(x);
    }
    void setTranstarty(int sy)
    {
        transtart.setY(sy);
    }
    void setTranstartx(int sx)
    {
        transtart.setX(sx);
    }
    void setTranslation(bool t){translation=t;}
    void setScaling(bool t){scaling=t;}
    void setFadeInTill(int t)
    {
        fadeInTill=t;
    }
    void setFadein(bool t)
    {
        fadein=t;
    }
    void setFadeOutFrom(int t)
    {
        fadeOutFrom=t;
    }
    void setFadeout(bool t)
    {
        fadeout=t;
    }

};

#endif // PICTURETRACK_H
