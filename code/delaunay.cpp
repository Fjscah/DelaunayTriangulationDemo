#include "delaunay.h"

Delaunay::Delaunay(QObject *parent)
    : QObject{parent}
{

}

void Delaunay::Delaunay_Contours(QVector<QPointF *> &points, QList<Triangle *> &triangles,const QList<QPolygonF> &polys)
{

    //-------CONSTRAIN EDGE -------
    // force triangle include constrain edge
    //poly to segment
    QList<Edge> segments;
    //Qlist<QpolygonF> polys
    foreach (auto poly, polys) {
        int length=poly.length();
        for(int j=0;j<length-1;j++)
        {
            segments.push_back(Edge(poly.at(j),poly.at(j+1)));
        }
        segments.push_back(Edge(poly.at(length-1),poly.at(0)));
    }
    Delaunay_Edges(points, triangles,segments);

    //--------REMOV EOUTER
    for (auto it = triangles.begin(); it != triangles.end();)
    {
        bool in_flag=false;
        QPointF center=(*it)->getcenter();
        foreach (auto poly, polys)
        {

            if (poly.containsPoint(center,Qt::WindingFill))
                in_flag=!in_flag;

        }
        if (!in_flag)
            it = triangles.erase(it);
        else
            it++;

    }

    //--------TRIANLGE LINK------
    QMap<QString,QList<Triangle*>> edgedict;

    for(int i=0;i<triangles.length();)
    {
        Triangle *tri=triangles.at(i);
        if (tri->isValid())
        {
            int length=0;
            bool remove_f=false;
            Triangle *tri_temp=nullptr;

            QString str=Edge2string(tri->getEdgelm());
            // remove repeat
            if (edgedict.contains(str))
            {
                 tri_temp= edgedict[str].at(0);
                if (*tri_temp==*tri)
                    remove_f=true;
            }
            str=Edge2string(tri->getEdgelr());
            if (edgedict.contains(str))
            {
                tri_temp = edgedict[str][0];
                if (*tri_temp==*tri)
                    remove_f=true;
            }
            str=Edge2string(tri->getEdgemr());
            if (edgedict.contains(str))
            {
                tri_temp = edgedict[str][0];
                if (*tri_temp==*tri)
                    remove_f=true;
            }

            if(!tri->isValid())
            {
                std::cout<<"**** error ***** repeat triangle \n";
                remove_f=true;
            }

            if(remove_f)
                triangles.removeAt(i);
            else

            {
                edgedict[Edge2string(tri->getEdgelm())].push_back(tri);
                edgedict[Edge2string(tri->getEdgelr())].push_back(tri);
                edgedict[Edge2string(tri->getEdgemr())].push_back(tri);
                i++;
            }
        }
        else
            triangles.removeAt(i);


    }
//    QMap使用Iterator.key()，和Iterator.value()方法获取第一个或第二个元素的值。
//    而std::map使用Iterator->first()， Iterator->second()来获取第一个或第二个元素的值
    QList<Triangle*> repeat_tris;
    for (auto iter = edgedict.begin(); iter != edgedict.end(); ++iter) {
            QList<Triangle*> tris=iter.value();
            if (tris.length()==2)// assume 2
            {
                tris.at(0)->linkNext(tris.at(1));
                tris.at(1)->linkNext(tris.at(0));
//                if (tris.at(0)->m_neinum!=tris.at(0)->neighbournum())
//                    std::cout<<"kkkk"<<std::endl;

            }
            else if (tris.length()>2)//3
            {
                std::cout <<"**** error ***** "<<tris.length()<<"  points in circle \n";

            }

        }

}

void Delaunay::Delaunay_Points(QVector<QPointF *> &points, QList<Triangle *> &triangles)
{
    //-------DALAUNAY TRIANGUALTION -------

    std::sort(points.begin(),points.end(),compare1);
    QList<Edge> edge_buffers;
    QList<Triangle*> temp_triangles;
    //get outer trangle

    // initial : include super triangle
    //temp_triangles.push_back(new Triangle(p0,p1,p2));
    // convexhull
    QVector<QPointF*> hull;
    convexHull(points, hull);
    HullDealunay(hull,temp_triangles);


    foreach (auto item, points) {//ellipse item
        edge_buffers.clear();
        for(int i=0;i<temp_triangles.length();) {
            Triangle* tri =temp_triangles.at(i);
            if (item->x()>tri->circle_centerx+sqrt(tri->radius_square))
                // point is right to circle
            {
                triangles.push_back(tri);
                temp_triangles.removeAt(i);
            }
            else if (tri->CircleContain(*item))
                // point in circle
            {
                temp_triangles.removeAt(i);
                edge_buffers.push_back(Edge(tri->left,tri->mid));
                edge_buffers.push_back(Edge(tri->right,tri->mid));
                edge_buffers.push_back(Edge(tri->left,tri->right));

            }
            else
                //else jump it wait next point to judge
                i++;

        }

        // remove double edge

        for (int i1=0;i1<edge_buffers.length();)

        {
            auto it1=edge_buffers.at(i1);
            bool hasRepeat = false;
            for (int i2 = i1+1; i2<edge_buffers.length();)
            {
                auto it2=edge_buffers.at(i2);
                if (it1 == it2)
                {
                    hasRepeat = true;

                    edge_buffers.removeAt(i2);
                    break;
                }
                else
                    i2++;
            }
            if (hasRepeat) edge_buffers.removeAt(i1);
            else i1++;
        }



        // create new triangle based on current point
        foreach (auto edge, edge_buffers) {
            if (Triangle::isValid(*item,edge))
            {
                Triangle * newt=new Triangle(*item,edge);
//                if (newt->isValid())
//
                temp_triangles.push_back(newt);
            }

        }

    }
    //merge temp triangles and triangles
    foreach (auto tri, temp_triangles) {
        triangles.push_back(tri);
    }
    return;
}


void Delaunay::Delaunay_Edges(QVector<QPointF *> &points, QList<Triangle *> &triangles,const QList<Edge> &segments)
{
    Delaunay_Points(points, triangles);
    // create dict point as key
    QMap<QString,QList<Triangle*>> nodedict;
    for (auto it = triangles.begin(); it != triangles.end();)
    {

        nodedict[point2string((*it)->left)].push_back(*it);
        nodedict[point2string((*it)->mid)].push_back(*it);
        nodedict[point2string((*it)->right)].push_back(*it);
        it++;

    }
    //-------CONSTRAIN EDGE -------
    //find cross edge in segment
    std::set<Edge> Unsegments;
    foreach (Edge edge, segments) {
        QPointF p=edge.p1;
        QString key= point2string(p);
        bool cover_f=false;
        QList<Triangle*> tris=nodedict.value(key);

        foreach (auto tri, tris) {

            Edge *e1=new Edge();
            Edge *e2=new Edge();
            int cnt=tri->getCrossEdge(edge,e1,e2);
            if (cnt){
                cover_f=true;

                break;
            }

        }

        if (cover_f) Unsegments.insert(edge);
    }

    //loop influenced triangles by edge , update for unsegment edge adding into delaunay triangulation
    foreach (Edge edge, Unsegments) {
        QList<Edge> crossedges;// edges that cross with unsegment
        QList<Triangle*> Influencedtris; // triangle which this crossedge construced
        getInfluencedtris(edge,crossedges,Influencedtris,nodedict);//p1 to p2
        if (Influencedtris.isEmpty()) continue;
        while(Influencedtris.length()>1)// until no influenced triangle, namely no cross edge for current edge
        {

            for (int i=0; i<crossedges.length();)

            {
                //diagonal
                Triangle* tri1=Influencedtris.at(i);
                Triangle* tri2=Influencedtris.at(i+1);
                Edge crossedge=crossedges.at(i);


                QPointF p1=tri1->PointToEdge(crossedge);
                QPointF p2=tri2->PointToEdge(crossedge);
                Edge diagonal=Edge(p1,p2);
                if (diagonal.crossEdge(crossedge)==2)//cross , exchange to diagnal
                {

                    //                    QVector<Triangle*>::iterator it1=find_if(triangles.begin(),triangles.end(),finder_t(tri1));//will remove from line a
                    //                    Triangle *it11=*it1;

                    //                    QVector<Triangle*>::iterator it2=find_if(triangles.begin(),triangles.end(),finder_t(tri2));//will updata from block a
                    //                     Triangle *it22=*it2;

                    if (crossedges.length()>i+1)//block a
                    {
                        Edge tempedge=crossedges.at(i+1);
                        if (tempedge.p1==crossedge.p1 ||tempedge.p2==crossedge.p1)
                        {
                            tri1->setPoint(p1,p2,crossedge.p2);
                            tri2->setPoint(p1,p2,crossedge.p1);

                        }

                        else
                        {
                            tri1->setPoint(p1,p2,crossedge.p1);
                            tri2->setPoint(p1,p2,crossedge.p2);
                        }


                    }
                    else
                    {
                        tri1->setPoint(p1,p2,crossedge.p1);
                        tri2->setPoint(p1,p2,crossedge.p2);
                    }
                    crossedges.removeAt(i);
                    Influencedtris.removeAt(i);//line a


                }
                else // no cross, keep it
                    i++;

            }
        }



    }



}

bool Delaunay::getInfluencedtris(const Edge &edge, QList<Edge> &crossedges, QList<Triangle *> &influencendtris, const QMap<QString, QList<Triangle *> > &nodedict)
{
    QPointF pstart=edge.p1;
    QPointF pend=edge.p2;
    Edge *crossedge;
    QString key= point2string(pstart);


    QList<Triangle*> tris=nodedict.value(key);

    Edge *e1=new Edge();
    Edge *e2=new Edge();
    int cnt=0;
    //begin
    foreach (auto tri, tris) {

        cnt=tri->getCrossEdge(edge,e1,e2);
        if (cnt==1)
        {
            influencendtris.push_back(tri);
            crossedge=e1;
            crossedges.push_back(*crossedge);
            break;
        }
    }
    // middle
    //QPointF p1=crossedge->p1;
    //QPointF p2=crossedge->p2;
    QPointF oldp=pstart;
    while(true)
    {
        QPointF p1=crossedge->p1;
        QPointF p2=crossedge->p2;
        key= point2string(p1);
        tris=nodedict.value(key);
        bool finded=false;
        foreach (auto tri, tris) {
            if (tri->PointContain(p2) && !tri->PointContain(oldp))
            {
                Edge *e1=new Edge();
                Edge *e2=new Edge();
                int cnt=(tri)->getCrossEdge(edge,e1,e2);
                if (cnt==2)
                {
                    if (*crossedge==*e1) crossedge=e2;
                    else crossedge=e1;
                    influencendtris.push_back(tri);

                    crossedges.push_back(*crossedge);
                    oldp=tri->PointToEdge(*crossedge);
                    finded=true;

                }
                else if (tri->PointContain(pend))//cnt=1
                {
                    finded=true;
                    influencendtris.push_back(tri);
                    return true;
                }
            }
        }
        if (!finded)
        {
            std::cout<<"error!!!!!! edge :"<<edge.p1.x()<<","<<edge.p1.y()<<
                       "   "<<edge.p2.x()<<","<<edge.p2.y()<<endl;
            return false;
        }
    }
}

bool compare1(const QPointF *p1, const QPointF *p2)
{
    if(p1->x()==p2->x())
        return p1->y()<p2->y();
    return p1->x()<p2->x();
}


void convexHull(const QList<QPointF*> & points, QVector<QPointF*> &hull)
{
    // There must be at least 3 points
    int n=points.length();
    if (n < 3) return;


    // Find the right most point
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i]->x() < points[l]->x())
            l = i;

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = l, q;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, q,
        // x) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p+1)%n;
        for (int i = 0; i < n; i++)
        {
            // If i is more counterclockwise than current q, then
            // update q
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

    } while (p != l);  // While we don't come to first point

    // Print Result
    //    for (int i = 0; i < hull.size(); i++)
    //        cout << "(" << hull[i]->x() << ", "
    //              << hull[i]->y() << ")\n";

}
void HullDealunay(QVector<QPointF*> &hull,QList<Triangle*>  &tries)
{
    for (int i=1;i<hull.length()-1;i++)
    {
        tries.push_back(new Triangle(*hull[0],*hull[i],*hull[i+1]));
    }
    for (int i=0;i<tries.length()-1;i++)
    {
        Triangle *tri=tries.at(i);
        tri->linkNext(tries.at(i+1));
        tries.at(i+1)->linkNext(tri);
    }


    bool change_f=false;
    int iter=0;
    do{
        iter++;
        change_f=false;
        foreach (auto tri, tries) {
            Triangle *tri1=  tri->nextTri();
            Triangle *tri2=tri->nextTri(tri1);
            Triangle *tri3=tri->nextTri(tri1,tri2);
            Triangle *trit=nullptr;
            int cnt=0;

            while(cnt!=3)
            {
                cnt++;
                if(cnt==1)trit=tri1;
                else if(cnt==2)trit=tri2;
                else if(cnt==3)trit=tri3;
                if(trit==nullptr) break;
                Edge comm_e=tri->commonEdge(*trit);
                QPointF p2=trit->PointToEdge(comm_e);
                QPointF p1=tri->PointToEdge(comm_e);
                if (tri->CircleContain(p2))
                {

                    //change to dialog
                    tri->setPoint(p1,p2,comm_e.p1);
                    trit->setPoint(p1,p2,comm_e.p2);

                    Triangle * nei1=tri->UpdataLink();
                    Triangle *nei2=trit->UpdataLink();
                    if(nei2!=nullptr)
                    {
                        tri->linkNext(nei2);
                        nei2->linkNext(tri);
                    }
                    if(nei1!=nullptr)
                    {
                        trit->linkNext(nei1);
                        nei1->linkNext(trit);
                    }
                    change_f=true;
                    break;
                }


            }
            if(change_f)break;


        }
    }while(change_f);
    foreach (auto tri1, tries)
    {
        tri1->clearNext();
    }

}
int orientation(const QPointF *p, const QPointF *q, const QPointF *r)
{
    // To find orientation of ordered triplet (p, q, r).
    // p->q , q->r
    // The function returns following values
    // 0 --> p, q and r are collinear
    // 1 --> Clockwise, q->r is right to p->q
    // 2 --> Counterclockwise
    double val = (q->y() - p->y()) * (r->x() - q->x()) -
            (q->x() - p->x()) * (r->y() - q->y());

    if (val == 0) return 0;  // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise

}



double PolygonArea(QPolygonF &poly)
{
//https://mathworld.wolfram.com/PolygonArea.html
//    Note that the area of a convex polygon is defined to be positive
//            if the points are arranged in a counterclockwise order,
//            and negative if they are in clockwise order (Beyer 1987)
   double area = 0;
   for (int i = 0; i < poly.length(); ++i) {
       QPointF p1=poly.at(i);
       QPointF p2=poly.at((i+1)%poly.length());
       area+= p1.x()*p2.y()-p2.x()*p1.y();
   }
   return abs(area) / 2;

}
