#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include "../model.h"
#include <QFileDialog>
#include <QListView>
#include <QDockWidget>
#include <QStandardItemModel>
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QSpinBox>

class MaterialWidget:public QDockWidget
{
    Q_OBJECT
public:
    MaterialWidget(QWidget* parent,Model*model);
    ~MaterialWidget();

    //允许拖入文件
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

    void addToolBar();
    void setView();

signals:
    void addtoList(QStandardItem* item);
    void addtoListSource(QStandardItem* item);
    void pictureIn(QStandardItem*item);
    void pictoTimeline(QStandardItem*item);
    void vidtoTimeline(QStandardItem*item);

public slots:
    void addClip();
    void addItem(QString filename);
    //选中右键菜单
    void onViewMenuRequest(const QPoint &pos);
    void onActionDelete();
    void onActionRename();
    void onActionToList();
    void onActionToSource();
    void ontoTimeline();

private:
    QWidget* w;
    QListView* view;
    Model* model;
    QRegularExpression*rxvideo;
    QRegularExpression*rxpicture;
    QRegularExpressionValidator* valvideo;
    QRegularExpressionValidator* valpicture;
    QVBoxLayout* layout;
    int &countVideo;
    int &countPicture;

    QSpinBox* setThumbPersant;
};



#endif // MATERIALWIDGET_H
