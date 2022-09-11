#ifndef TIMEBOX_H
#define TIMEBOX_H

#include <QSpinBox>

class Timebox:public QSpinBox
{
    Q_OBJECT
public:
    Timebox() {}
    Timebox(QWidget*parent);
    ~Timebox() {}

protected:
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;
    QValidator::State validate(QString&text,int &pos)const;
};

#endif // TIMEBOX_H
