#ifndef GRAPHICSPOINT_H
#define GRAPHICSPOINT_H

#include <QWidget>
#include "stdxfm.h"


class GraphicsPoint :  public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    explicit GraphicsPoint(QGraphicsEllipseItem *parent = nullptr);
    explicit GraphicsPoint(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr);
protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
       void clicked_signal(GraphicsPoint *item);
        void cc();

};

#endif // GRAPHICSPOINT_H
