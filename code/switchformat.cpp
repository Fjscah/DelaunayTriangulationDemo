#include "switchformat.h"



QString point2string(const QPointF &p)
{
    return QString::asprintf("%.1f,%.1f",p.x(),p.y());
}
QString Edge2string(const Edge &e)
{
    return QString::asprintf("%.1f,%.1f,%.1f,%.1f",e.p1.x(),e.p1.y(),e.p2.x(),e.p2.y());
}


