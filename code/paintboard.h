#ifndef PAINTBOARD_H
#define PAINTBOARD_H

#include <QWidget>
#include <QGraphicsView>
#include "stdxfm.h"

#include <QRgb>
#include "graphicspoint.h"
#include "delaunay.h"
#include <QtGlobal>
#include <QLine>

// cannot use  scene.clear
//Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged
//Removes the item item and all its children from the scene. The ownership of item is passed on to the caller (i.e., QGraphicsScene will no longer delete item when destroyed)
//
class ImageProcessor;
class PaintBoard : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PaintBoard(QWidget *parent = nullptr);
    QVector<QPointF*> points;
    QVector<GraphicsPoint*> pointitems;

    QList<Triangle*> triangles;
    QMap<QString,QList<Triangle*>> nodedict;
    QList<QGraphicsPolygonItem*> triangleitems;

    QList<Edge> segments;
    QVector<QGraphicsPolygonItem*> segmentitems;

    QList<QVector<QGraphicsPolygonItem*>> polyitemss;

    QGraphicsScene *scene;
    QRect rect;
    QGraphicsPolygonItem *curitem=nullptr;
    QPointF firstpoint;
    QPointF curpoint;
    bool Is_draw=false;

private:
    int m_maxX=std::numeric_limits<int>::max();
    int m_minX=0;
    int m_minY=0;
    int m_maxY=m_maxX;

    void ResetBorder();
    void CalcuBorder();


signals:
    void sig_CurrentPos(float x, float y);
public:

    bool contour_show=true;
    bool triangle_show=true;
    bool spine_show=false;
    bool img_show=true;

    void GeneratePoints(int num);
    void GeneratePointsFromPoly(const QPolygonF &poly);

    void Bowyer();//triangulation for given points
    void ConstrainBowyer();//triangulation for given points and edges
    void EachBowyer();//triangulation for hierachary contours

    void CleanAll();
    void CleanResult();
    void clearCurrentitem();

    void updateTriangle();
    void showTriangle(bool b);

    QGraphicsPixmapItem * pixitem=nullptr;
    QGraphicsPixmapItem * pixitem_backup=nullptr;


protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

};

bool compare2(const GraphicsPoint*p1,const GraphicsPoint*p2);





//定义查找规则
typedef struct finder_t
{
    finder_t(Triangle* tri) : m_item(tri) { }
    bool operator()(Triangle* p ) { return ((void *) p== m_item); }
    void * m_item;
}finder_t;

#endif // PAINTBOARD_H
