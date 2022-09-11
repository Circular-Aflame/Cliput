#include "materialwidget.h"
#include <QDir>
#include <QMenu>
#include <QToolBar>

MaterialWidget::MaterialWidget(QWidget* parent,Model*model)
    :QDockWidget(parent),model(model),countVideo(model->countVideo),countPicture(model->countPicture)
{
    this->setWindowTitle("Material");
    //设置主窗口
    w=new QWidget(this);
    this->setWidget(w);

    //初始化view和model
    view=new QListView(this);
    setView();

    //添加菜单
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onViewMenuRequest(QPoint)));

    //允许拖入文件
    rxvideo=new QRegularExpression(".*\\.(mp4|MP4|)");
    valvideo=new QRegularExpressionValidator(*rxvideo,0);
    rxpicture=new QRegularExpression(".*\\.(jpg|png|)");
    valpicture=new QRegularExpressionValidator(*rxpicture,0);
    this->setAcceptDrops(true);

    //窗口布局
    layout=new QVBoxLayout(w);
    layout->addWidget(view);
    addToolBar();
}

void MaterialWidget::setView()
{

    view->setWrapping(true);
    view->setViewMode(QListView::IconMode);
    view->setMovement(QListView::Static);
    view->setResizeMode(QListView::Adjust);
    view->setGridSize(QSize(165,115));
    view->setIconSize(QSize(160,90));

    view->setModel(model);
}

void MaterialWidget::addToolBar()
{
    QToolBar*toolbar=new QToolBar(w);
    layout->addWidget(toolbar,Qt::BottomToolBarArea);
    toolbar->addAction("+",this,&MaterialWidget::addClip);
    toolbar->addAction("-",this,&MaterialWidget::onActionDelete);
    toolbar->addAction("List",this,&MaterialWidget::onActionToList);
    toolbar->addAction("So",this,&MaterialWidget::onActionToSource);
    toolbar->addAction("Clip",this,&MaterialWidget::ontoTimeline);

    toolbar->addAction("Renew",model,&Model::onrenew);
    setThumbPersant=new QSpinBox(this);
    setThumbPersant->setToolTip("Set Thumbnail Persant");
    setThumbPersant->setKeyboardTracking(false);
    setThumbPersant->setMinimum(0);
    setThumbPersant->setMaximum(100);
    setThumbPersant->setValue(20);
    setThumbPersant->setSingleStep(5);
    setThumbPersant->setSuffix("%");
    connect(setThumbPersant,SIGNAL(valueChanged(int)),model,SLOT(setThumbPersent(int)));
    toolbar->addWidget(setThumbPersant);
}

void MaterialWidget::onViewMenuRequest(const QPoint& pos)
{
    QModelIndex curin=view->indexAt(pos);
    if(curin.isValid())
    {
        QMenu*menu=new QMenu(this);
        menu->addAction("Delete",this,&MaterialWidget::onActionDelete);
        menu->addAction("Rename",this,&MaterialWidget::onActionRename);
        menu->addAction("Add to Playlist",this,&MaterialWidget::onActionToList);
        menu->addAction("Add to Source",this,&MaterialWidget::onActionToSource);
        menu->addAction("Add to Timeline",this,&MaterialWidget::ontoTimeline);
        menu->exec(QCursor::pos());
    }
}

void MaterialWidget::ontoTimeline()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    QString s=model->itemFromIndex(curin)->text();
    int pos=0;
    if(valvideo->validate(s,pos)==QValidator::Acceptable)
    {
        emit vidtoTimeline(model->itemFromIndex(curin));
    }
    else
    {
        emit pictoTimeline(model->itemFromIndex(curin));
    }
}

void MaterialWidget::onActionDelete()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    model->removeRow(curin.row(),curin.parent());
    if(curin.row()<countVideo)
    {
        countVideo--;
    }
    else
    {
        countPicture--;
    }
}

void MaterialWidget::onActionRename()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    view->edit(curin);
}

void MaterialWidget::onActionToList()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    QString s=model->itemFromIndex(curin)->text();
    int pos=0;
    if(valvideo->validate(s,pos)==QValidator::Acceptable)
    {
        emit addtoList(model->itemFromIndex(curin));
    }
}

void MaterialWidget::onActionToSource()
{
    QModelIndex curin=view->currentIndex();
    if(!curin.isValid())
    {
        return;
    }
    QString s=model->itemFromIndex(curin)->text();
    int pos=0;
    if(valvideo->validate(s,pos)==QValidator::Acceptable)
    {
        emit addtoListSource(model->itemFromIndex(curin));
    }
}

void MaterialWidget::addClip()
{
    //打开文件窗口，TODO：完善文件格式
    QList<QString> urls=QFileDialog::getOpenFileNames(
                this,
                tr("Select media file"),
                QDir::currentPath(),
                tr("Video files (*.mp4 *.MP4);;Picture files(*.jpg *.png)"));
    for(auto url:urls)
    {
        if(url.contains("."))
        {
            addItem(url);
        }
    }
}

void MaterialWidget::addItem(QString filename)
{
    QString s=filename.split('/').last();
    QStandardItem *item=new QStandardItem(s);
    item->setData(filename);
    int pos=0;
    if(valvideo->validate(s,pos)==QValidator::Acceptable)
    {
        model->addVideo(countVideo,item);
        countVideo++;
    }
    if(valpicture->validate(s,pos)==QValidator::Acceptable)
    {
        model->insertRow(countVideo+countPicture,item);
        countPicture++;
        item->setIcon(QIcon(filename));
        emit pictureIn(item);
    }
}

//判断格式后允许拖入
void MaterialWidget::dragEnterEvent(QDragEnterEvent* event)
{
    QList<QUrl> urls=event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    QString s;
    int pos=0;
    for(auto url:urls)
    {
        s=url.toLocalFile();
        if(valpicture->validate(s,pos)==QValidator::Acceptable||valvideo->validate(s,pos)==QValidator::Acceptable)
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore();
}

//拖入后加入列表事件
void MaterialWidget::dropEvent(QDropEvent* event)
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
        if(valpicture->validate(s,pos)==QValidator::Acceptable||valvideo->validate(s,pos)==QValidator::Acceptable)
        {
            addItem(s);
        }
    }
}

MaterialWidget::~MaterialWidget()
{

}
