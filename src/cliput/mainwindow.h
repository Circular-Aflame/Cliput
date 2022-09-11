#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./widgets/listwidget.h"
#include "./widgets/sourcewidget.h"
#include "./widgets/previewwidget.h"
#include "./widgets/materialwidget.h"
#include "./widgets/timelinewidget.h"
#include "model.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createMenu();
    void createToolBar();
    //窗口布局
    void layDocks();

private:
    Model*model;
    SourceWidget* sourceWidget;
    ListWidget* listWidget;
    PreviewWidget* previewWidget;
    MaterialWidget* materialWidget;
    TimelineWidget* timelineWidget;
};
#endif // MAINWINDOW_H
