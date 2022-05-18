#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <QObject>
#include "stdxfm.h"
#include "triangle.h"
#include "switchformat.h"

class Delaunay : public QObject
{
    Q_OBJECT
public:
    explicit Delaunay(QObject *parent = nullptr);

signals:
public:
    static void Delaunay_Contours(QVector<QPointF*> &points, QList<Triangle*> &tris,const QList<QPolygonF> &polys);
    static void Delaunay_Points(QVector<QPointF*> &points, QList<Triangle*> &tris);
    static void Delaunay_Edges(QVector<QPointF *> &points, QList<Triangle *> &triangles,const QList<Edge> &segments);
    static bool getInfluencedtris(const Edge &edge,QList<Edge> &crossedges,QList<Triangle*> & influencendtris,const QMap<QString,QList<Triangle*>> &nodedict);
};

bool compare1(const QPointF *p1,const QPointF *p2);
//https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
int orientation(const QPointF *p, const QPointF *q, const QPointF *r);
// Prints convex hull of a set of n points.
void convexHull(const QList<QPointF*> & points, QVector<QPointF*> &hull);
void HullDealunay(QVector<QPointF*> &hull,QList<Triangle*>  &tries);

double PolygonArea(QPolygonF &poly);
#endif // DELAUNAY_H
