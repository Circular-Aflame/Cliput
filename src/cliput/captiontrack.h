#ifndef CAPTIONTRACK_H
#define CAPTIONTRACK_H

#include "trackitem.h"
#include "subtitle.h"

class CaptionTrack:public TrackItem
{
    Q_OBJECT
public:
    CaptionTrack(QString text,int duration,TimelineScene *s,int importm,int exportm)
        :TrackItem(text,duration,Caption,s,importm,exportm)
    {
        toplim=height*3.5;
        botlim=height*5.5;
        mos=1000000;
        sub=new Subtitle();
        sub->setPlainText(text);
        subti=true;
    }
    CaptionTrack(QString text,int duration,TimelineScene *s,int importm,int exportm,Subtitle*subt)
        :TrackItem(text,duration,Caption,s,importm,exportm)
    {
        toplim=height*3.5;
        botlim=height*4.5;
        mos=1000000;
        sub=subt;
        subti=false;
    }
    ~CaptionTrack()
    {
        delete sub;
    }
    Subtitle* sub;
    bool subti;
};

#endif // CAPTIONTRACK_H
