#ifndef TOOLACTION_H
#define TOOLACTION_H

#include <QAction>

class ToolAction:public QAction
{
    Q_OBJECT
public:
    ToolAction() {}
    ToolAction(QAction* act):QAction(act->text(),act->parent())
    {
        this->setCheckable(true);
        this->setChecked(true);
        connect(this,SIGNAL(toggled(bool)),act,SLOT(setVisible(bool)));
    }
    ~ToolAction(){}
};

#endif // TOOLACTION_H
