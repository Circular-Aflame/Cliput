#include "timebox.h"
#include "algor.h"

Timebox::Timebox(QWidget*parent):QSpinBox(parent)
{
    this->setSingleStep(5000);
    this->setKeyboardTracking(false);
}

int Timebox::valueFromText(const QString &text) const
{
    return Mstime::hmstoMs(text);
}

QString Timebox::textFromValue(int value) const
{
    return Mstime::toHMS(value);
}

QValidator::State Timebox::validate(QString&,int&) const
{
    return QValidator::Acceptable;
}
