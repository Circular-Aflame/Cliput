#ifndef TIMELINESCENE_H
#define TIMELINESCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class TrackItem;

class TimelineScene:public QGraphicsScene
{
    Q_OBJECT
public slots:
    //比例刷新
    void setStepIndex(int);
    void refresh();

signals:
    void markerChanged(int);
    void review();

protected:
    //marker点击移动
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    TimelineScene(QObject*parent=nullptr);
    ~TimelineScene() {}

    //过滤获取轨道
    QList<TrackItem*> selectedTracks();
    QList<TrackItem*> allTracks();
    QList<TrackItem*> filterTracks(const QList<QGraphicsItem*>&items);

    //比例系数
    constexpr const static int steps[10]={10,50,100,300,600,1200,3000,4800,7200,12000};

    int step;
    int ind;
    int trackheight;
    int lastms;
};

#endif // TIMELINESCENE_H
