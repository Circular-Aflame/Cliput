#include "listwidget.h"
#include <QDir>
#include <QMenu>
#include <QToolBar>
#include <QHeaderView>

ListWidget::ListWidget(QWidget* parent,Model* model)
    :QDockWidget(parent),model(model)
{
    this->setWindowTitle("List");
    //设置主窗口
    w=new QWidget(this);
    this->setWidget(w);

    //初始化view和model
    view=new QTreeView(this);
    model->setHorizontalHeaderLabels(QStringList()<<"Clip"<<"Duration");
    view->header()->setStyleSheet("QHeaderView::section{background:#D3D3D3;}");
    view->setModel(model);
    view->setIconSize(QSize(80,45));
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //添加菜单
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onViewMenuRequest(QPoint)));

    //允许拖入文件
    rx=new QRegularExpression(".*\\.(mp4|MP4|)");
    val=new QRegularExpressionValidator(*rx,0);
    this->setAcceptDrops(true);

    //窗口布局
    layout=new QVBoxLayout(w);
    layout->addWidget(view);
    addToolBar();
}

void ListWidget::onAddtoTrack()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    emit addtoTrack(model->itemFromIndex(curin.siblingAtColumn(0)));
}

void ListWidget::onPictureIn(QStandardItem*item)
{
    QModelIndex curin=model->indexFromItem(item);
    view->setRowHidden(curin.row(),curin.parent(),true);
}

void ListWidget::onAddtoList(QStandardItem*item)
{
    QModelIndex curin=model->indexFromItem(item);
    view->setRowHidden(curin.row(),curin.parent(),false);
}

void ListWidget::onAddtoListSource(QStandardItem*item)
{
    QModelIndex curin=model->indexFromItem(item);
    view->setRowHidden(curin.row(),curin.parent(),false);
    emit addtoSource(item->data().toString());
}

void ListWidget::addToolBar()
{
    QToolBar*toolbar=new QToolBar(w);
    layout->addWidget(toolbar,Qt::BottomToolBarArea);
    toolbar->addAction("+",this,&ListWidget::addClip);
    toolbar->addAction("-",this,&ListWidget::onActionDelete);
    toolbar->addAction("So",this,&ListWidget::onActionToSource);
    toolbar->addAction("Clip",this,&ListWidget::onAddtoTrack);
}

void ListWidget::onViewMenuRequest(const QPoint& pos)
{
    QModelIndex curin=view->indexAt(pos);
    if(curin.isValid())
    {
        QMenu*menu=new QMenu(this);
        menu->addAction("Delete",this,&ListWidget::onActionDelete);
        menu->addAction("Add to Source",this,&ListWidget::onActionToSource);
        menu->addAction("Add to Timeline",this,&ListWidget::onAddtoTrack);
        menu->exec(QCursor::pos());
    }
}

void ListWidget::onActionDelete()
{
    QModelIndex curin=view->currentIndex();
    view->setRowHidden(curin.row(),curin.parent(),true);
}

void ListWidget::onActionToSource()
{
    QModelIndex curin=view->currentIndex();
    if(curin.isValid())
    {
        emit addtoSource(model->itemFromIndex(curin.siblingAtColumn(0))->data().toString());
    }
}

void ListWidget::addClip()
{
    //打开文件窗口，TODO：完善文件格式
    QList<QString> urls=QFileDialog::getOpenFileNames(
                this,
                tr("Select media file"),
                QDir::currentPath(),
                tr("Video files (*.mp4 *.MP4)"));
    for(auto url:urls)
    {
        if(url.contains("."))
        {
            addItem(url);
        }
    }
}

void ListWidget::addItem(QString filename)
{
    emit addVideoItem(filename);
}

//判断格式后允许拖入
void ListWidget::dragEnterEvent(QDragEnterEvent* event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    int pos=0;
    QString s;
    for(auto url:urls)
    {
        s=url.toLocalFile();
        if(val->validate(s,pos)==QValidator::Acceptable)
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

//拖入后加入列表事件
void ListWidget::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    int pos=0;
    QString s;
    for(auto url:urls)
    {
        s=url.toLocalFile();
        if(val->validate(s,pos)==QValidator::Acceptable)
        {
            addItem(s);
        }
    }
}

ListWidget::~ListWidget()
{

}
