#ifndef PICON_H
#define PICON_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

//预览窗口中图片物件
class Picon:public QGraphicsItem
{
public:
    Picon();
    ~Picon() {}
    QString url;
    QString text;
    bool pressed;
    QPointF oldMousePos,oldPos;
    int cursortype;
    //四方伸缩框
    QRectF *leftbox,*rightbox,*topbox,*botbox;
    QRectF *ltb,*rtb,*lbb,*rbb;
    bool left,right,top,bot;
    bool lt,rt,lb,rb;
    int width,height;
    double ow,oh;
    int box;
    QPixmap*map;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setUrl(QString url);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;



};

#endif // PICON_H
