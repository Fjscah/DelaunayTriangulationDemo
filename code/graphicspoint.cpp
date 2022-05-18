#include "graphicspoint.h"

GraphicsPoint::GraphicsPoint(QGraphicsEllipseItem *parent)
    : QGraphicsEllipseItem{parent}
{

}

GraphicsPoint::GraphicsPoint(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    :QGraphicsEllipseItem(x,y, w, h,parent)
{

}

void GraphicsPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit cc();
    emit clicked_signal(this);
    QGraphicsEllipseItem::mouseMoveEvent(event);
}
