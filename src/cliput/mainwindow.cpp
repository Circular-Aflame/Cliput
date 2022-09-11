#include "mainwindow.h"
#include "toolaction.h"
//#include "trackitem.h"
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/pics/icon.png"));
    this->setStyleSheet("background-color:lightgrey");
    model=new Model(this);
    sourceWidget=new SourceWidget(this);
    listWidget=new ListWidget(this,model);
    previewWidget=new PreviewWidget(this);
    materialWidget=new MaterialWidget(this,model);
    timelineWidget=new TimelineWidget(this);

    //连接
    connect(listWidget,SIGNAL(addtoSource(QString)),sourceWidget,SLOT(addVideo(QString)));
    connect(listWidget,SIGNAL(addVideoItem(QString)),materialWidget,SLOT(addItem(QString)));
    connect(materialWidget,SIGNAL(addtoList(QStandardItem*)),listWidget,SLOT(onAddtoList(QStandardItem*)));
    connect(materialWidget,SIGNAL(addtoListSource(QStandardItem*)),listWidget,SLOT(onAddtoListSource(QStandardItem*)));
    connect(materialWidget,SIGNAL(pictureIn(QStandardItem*)),listWidget,SLOT(onPictureIn(QStandardItem*)));
    connect(listWidget,SIGNAL(addtoTrack(QStandardItem*)),timelineWidget,SLOT(addVideoTrack(QStandardItem*)));
    connect(materialWidget,SIGNAL(vidtoTimeline(QStandardItem*)),timelineWidget,SLOT(addVideoTrack(QStandardItem*)));
    connect(materialWidget,SIGNAL(pictoTimeline(QStandardItem*)),timelineWidget,SLOT(addPictureTrack(QStandardItem*)));
    connect(timelineWidget,SIGNAL(newClipout(QString)),materialWidget,SLOT(addItem(QString)));
    connect(previewWidget,SIGNAL(isplaying(bool)),timelineWidget,SLOT(previewPlay(bool)));
    connect(timelineWidget,SIGNAL(tracktoPreview(QList<TrackItem*>&)),previewWidget,SLOT(trackinPreview(QList<TrackItem*>&)));
    connect(timelineWidget,SIGNAL(previewStop()),previewWidget,SLOT(previewstop()));
    connect(previewWidget,SIGNAL(newSubcreated(Subtitle*)),timelineWidget,SLOT(addCaptionTrack(Subtitle*)));
    connect(timelineWidget,SIGNAL(deleteItems(QList<TrackItem*>&)),previewWidget,SLOT(tracktoDelete(QList<TrackItem*>&)));
    connect(previewWidget,SIGNAL(newPiconcreated(Picon*)),timelineWidget,SLOT(addPiconTrack(Picon*)));
    connect(timelineWidget,SIGNAL(newPiconCreated(Picon*)),previewWidget,SLOT(onPiconcreated(Picon*)));

    //窗口初始化
    layDocks();
    //菜单
    createMenu();
    //工具栏
    createToolBar();
}

void MainWindow::layDocks()
{
    //删除中心窗口
    QWidget* p = takeCentralWidget();
    if (p)
    {
        delete p;
    }
    //允许镶嵌布局
    this->setDockNestingEnabled(true);
    this->resize(1080,720);
    //布局
    this->addDockWidget(Qt::LeftDockWidgetArea,materialWidget);
    this->splitDockWidget(materialWidget,timelineWidget,Qt::Vertical);
    this->splitDockWidget(materialWidget,previewWidget,Qt::Horizontal);
    this->splitDockWidget(timelineWidget,listWidget,Qt::Horizontal);
    this->splitDockWidget(previewWidget,sourceWidget,Qt::Horizontal);
}

void MainWindow::createMenu()
{
    QMenuBar* menubar=new QMenuBar(this);
    this->setMenuBar(menubar);
    QMenu* menuFile=new QMenu("File",this);
    menubar->addMenu(menuFile);
    menuFile->addAction("Open Video",listWidget,&ListWidget::addClip);
    menuFile->addAction("Open files",materialWidget,&MaterialWidget::addClip);
    menuFile->addAction("Import srt Caption",timelineWidget,&TimelineWidget::addSRTCaption);


    QMenu* menuEdit=new QMenu("Edit",this);
    menubar->addMenu(menuEdit);
    menuEdit->addAction("Cut",timelineWidget,&TimelineWidget::onActionCut);
    menuEdit->addAction("Clip out",timelineWidget,&TimelineWidget::onActionClipout);
    menuEdit->addAction("Clip combine",timelineWidget,&TimelineWidget::onActionCombine);
    menuEdit->addAction("Vedio out",timelineWidget,&TimelineWidget::onActionVideoout);
    menuEdit->addAction("Picture Effect",timelineWidget,&TimelineWidget::onAddEffect);

}

void MainWindow::createToolBar()
{
    //菜单栏中加入工具栏选择性显示控件
    QMenu* toolDisplay=new QMenu("Toolbar",this);
    this->menuBar()->addMenu(toolDisplay);

    //加入工具栏
    QToolBar*toolbar=new QToolBar(this);
    toolbar->setWindowTitle("toolbar");
    toolbar->setMovable(false);
    this->addToolBar(toolbar);

    //按钮及可选
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Open",materialWidget,&MaterialWidget::addClip)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("SRT",timelineWidget,&TimelineWidget::addSRTCaption)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Cut",timelineWidget,&TimelineWidget::onActionCut)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Clip",timelineWidget,&TimelineWidget::onActionClipout)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Combine",timelineWidget,&TimelineWidget::onActionCombine)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Export",timelineWidget,&TimelineWidget::onActionVideoout)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Effect",timelineWidget,&TimelineWidget::onAddEffect)));
    toolDisplay->addAction(new ToolAction(toolbar->addAction("Source",listWidget,&ListWidget::onActionToSource)));
}

MainWindow::~MainWindow()
{
}

