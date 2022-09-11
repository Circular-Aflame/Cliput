#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "../model.h"
#include <QFileDialog>
#include <QTreeView>
#include <QDockWidget>
#include <QStandardItemModel>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

class ListWidget:public QDockWidget
{
    Q_OBJECT
public:
    ListWidget() {}
    ListWidget(QWidget* parent,Model* model);
    ~ListWidget();

    //允许拖入文件
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    void addToolBar();

signals:
    void addtoSource(QString filename);
    void addVideoItem(QString filename);
    void addtoTrack(QStandardItem*item);

public slots:
    void addClip();
    void addItem(QString filename);
    //选中右键菜单
    void onViewMenuRequest(const QPoint &pos);
    void onActionDelete();
    void onActionToSource();

    void onAddtoList(QStandardItem* item);
    void onAddtoListSource(QStandardItem*item);
    void onAddtoTrack();

    void onPictureIn(QStandardItem* item);

private:
    QWidget* w;
    QTreeView* view;
    Model* model;
    QRegularExpression*rx;
    QRegularExpressionValidator* val;
    QVBoxLayout* layout;
};



#endif // LISTWIDGET_H
