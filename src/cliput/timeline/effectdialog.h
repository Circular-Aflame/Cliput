#ifndef EFFECTDIALOG_H
#define EFFECTDIALOG_H

#include "picturetrack.h"
#include <QDialog>
#include <QGridLayout>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>

class EffectDialog:public QDialog
{
    Q_OBJECT
public:
    EffectDialog(QWidget*parent,PictureTrack*p):QDialog(parent),pt(p)
    {
        setWindowTitle("Effects");
        layout=new QGridLayout(this);
        //fade in
        fadein=new QGroupBox("Fade in",this);
        fadein->setCheckable(true);
        QVBoxLayout*fadeinLayout=new QVBoxLayout(fadein);
        fadeinTo=new QSpinBox;
        fadeinLayout->addWidget(fadeinTo);
        fadeinTo->setMinimum(0);
        fadeinTo->setMaximum(pt->exportms-pt->importms);
        fadeinTo->setSingleStep(1000);
        fadeinTo->setSuffix("ms");
        if(pt->fadein)
        {
            fadein->setChecked(true);
            fadeinTo->setValue(pt->fadeInTill);
        }
        else
        {
            fadein->setChecked(false);
            fadeinTo->setValue(0);
        }
        connect(fadeinTo,SIGNAL(valueChanged(int)),pt,SLOT(setFadeInTill(int)));
        connect(fadein,SIGNAL(toggled(bool)),pt,SLOT(setFadein(bool)));
        //fade out
        fadeout=new QGroupBox("Fade out",this);
        fadeout->setCheckable(true);
        QVBoxLayout*fadeoutLayout=new QVBoxLayout(fadeout);
        fadeoutFrom=new QSpinBox;
        fadeoutLayout->addWidget(fadeoutFrom);
        fadeoutFrom->setMinimum(0);
        fadeoutFrom->setMaximum(pt->exportms-pt->importms);
        fadeoutFrom->setSingleStep(1000);
        fadeoutFrom->setSuffix("ms");
        if(pt->fadeout)
        {
            fadeout->setChecked(true);
            fadeoutFrom->setValue(pt->fadeOutFrom);
        }
        else
        {
            fadeout->setChecked(false);
            fadeoutFrom->setValue(0);
        }
        connect(fadeoutFrom,SIGNAL(valueChanged(int)),pt,SLOT(setFadeOutFrom(int)));
        connect(fadeout,SIGNAL(toggled(bool)),pt,SLOT(setFadeout(bool)));
        //translation
        translation=new QGroupBox("Translation",this);
        translation->setCheckable(true);
        QVBoxLayout*tlay=new QVBoxLayout(translation);
        transtartx=new QSpinBox;
        transtartx->setPrefix("sx:");
        transtartx->setMaximum(1280);
        transtartx->setSingleStep(10);
        tlay->addWidget(transtartx);
        transtarty=new QSpinBox;
        transtarty->setPrefix("sy:");
        transtarty->setMaximum(720);
        transtarty->setSingleStep(10);
        tlay->addWidget(transtarty);
        transendx=new QSpinBox;
        transendx->setPrefix("ex:");
        transendx->setMaximum(1280);
        transendx->setSingleStep(10);
        tlay->addWidget(transendx);
        transendy=new QSpinBox;
        transendy->setPrefix("ey:");
        transendy->setMaximum(720);
        transendy->setSingleStep(10);
        tlay->addWidget(transendy);
        if(pt->translation)
        {
            translation->setChecked(true);
            transtartx->setValue(pt->transtart.x());
            transtarty->setValue(pt->transtart.y());
            transendx->setValue(pt->transend.x());
            transendy->setValue(pt->transend.y());
        }
        else
        {
            translation->setChecked(false);
            transtartx->setValue(pt->pic->x());
            transtarty->setValue(pt->pic->y());
            transendx->setValue(pt->pic->x());
            transendy->setValue(pt->pic->y());
        }
        connect(translation,SIGNAL(toggled(bool)),pt,SLOT(setTranslation(bool)));
        connect(transtartx,SIGNAL(valueChanged(int)),pt,SLOT(setTranstartx(int)));
        connect(transtarty,SIGNAL(valueChanged(int)),pt,SLOT(setTranstarty(int)));
        connect(transendx,SIGNAL(valueChanged(int)),pt,SLOT(setTransendx(int)));
        connect(transendy,SIGNAL(valueChanged(int)),pt,SLOT(setTransendy(int)));
        //scaling
        scaling=new QGroupBox("Scaling",this);
        scaling->setCheckable(true);
        QVBoxLayout*slay=new QVBoxLayout(scaling);
        scalstartw=new QSpinBox;
        scalstartw->setPrefix("sw:");
        scalstartw->setMaximum(1280);
        scalstartw->setSingleStep(10);
        slay->addWidget(scalstartw);
        scalstarth=new QSpinBox;
        scalstarth->setPrefix("sh:");
        scalstarth->setMaximum(720);
        scalstarth->setSingleStep(10);
        slay->addWidget(scalstarth);
        scalendw=new QSpinBox;
        scalendw->setPrefix("ew:");
        scalendw->setMaximum(1280);
        scalendw->setSingleStep(10);
        slay->addWidget(scalendw);
        scalendh=new QSpinBox;
        scalendh->setPrefix("eh:");
        scalendh->setMaximum(720);
        scalendh->setSingleStep(10);
        slay->addWidget(scalendh);
        if(pt->scaling)
        {
            scaling->setChecked(true);
            scalstartw->setValue(pt->scalstart.width());
            scalstarth->setValue(pt->scalstart.height());
            scalendw->setValue(pt->scalend.width());
            scalendh->setValue(pt->scalend.height());
        }
        else
        {
            scaling->setChecked(false);
            scalstartw->setValue(pt->pic->width);
            scalstarth->setValue(pt->pic->height);
            scalendw->setValue(pt->pic->width);
            scalendh->setValue(pt->pic->height);
        }
        connect(scaling,SIGNAL(toggled(bool)),pt,SLOT(setScaling(bool)));
        connect(scalstartw,SIGNAL(valueChanged(int)),pt,SLOT(setscalstartw(int)));
        connect(scalstarth,SIGNAL(valueChanged(int)),pt,SLOT(setscalstarth(int)));
        connect(scalendw,SIGNAL(valueChanged(int)),pt,SLOT(setscalendw(int)));
        connect(scalendh,SIGNAL(valueChanged(int)),pt,SLOT(setscalendh(int)));




        layout->addWidget(fadein,0,0,1,1);
        layout->addWidget(fadeout,0,1,1,1);
        layout->addWidget(translation,1,0,2,1);
        layout->addWidget(scaling,1,1,2,1);

    }

private:
    PictureTrack* pt;
    QGridLayout*layout;
    QGroupBox *fadein,*fadeout,*translation,*scaling;
    QSpinBox *fadeinTo,*fadeoutFrom,*transtartx,*transtarty;
    QSpinBox *transendx,*transendy;
    QSpinBox *scalstartw,*scalstarth,*scalendw,*scalendh;

};

#endif // EFFECTDIALOG_H
