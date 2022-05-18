#include "triangle.h"
#include <iostream>

Triangle::Triangle()
{

}

Triangle::Triangle(QPointF pp1, QPointF pp2, QPointF pp3)
{

    setPoint(pp1,pp2,pp3);

}

Triangle::Triangle(const QGraphicsPolygonItem &p)
{
    QPolygonF poly=p.polygon();
    QPointF pp1=poly.at(0);
    QPointF pp2=poly.at(1);
    QPointF pp3=poly.at(2);

    setPoint(pp1,pp2,pp3);


}

bool Triangle::isValid(const QPointF &p, const Edge &e)
{
   QPointF v1= p-e.p1;
   QPointF v2=p-e.p2;
//   if (v1.y()==0)
//       return v2.y()!=0;
//   if (v2.y()==0)
//       return v1.y()!=0;
   return v1.x()*v2.y()!=v1.y()*v2.x();
}

Triangle::Triangle(const Triangle &t):
    left(t.left),mid(t.mid),right(t.right),
    radius_square(t.radius_square),circle_centerx(t.circle_centerx),circle_centery(t.circle_centery)
{

}

Triangle &Triangle::operator=(const Triangle &t)
{
    left = t.left; mid = t.mid; right = t.right;
    radius_square = t.radius_square; circle_centerx = t.circle_centerx, circle_centery = t.circle_centery;
    return *this;

}

bool Triangle::operator ==(const Triangle &t) const
{
return left==t.left &&mid==t.mid && right==t.right;
}

void Triangle::setPoint(QPointF pp1, QPointF pp2, QPointF pp3)
{

    if(pp1.x()>pp2.x())swapPoint(pp1,pp2);
    else if(pp1.x()==pp2.x() && pp1.y()>pp2.y()) swapPoint(pp1,pp2);
    if(pp2.x()>pp3.x())swapPoint(pp2,pp3);
    else if(pp2.x()==pp3.x() && pp2.y()>pp3.y()) swapPoint(pp2,pp3);
    //pp3 is right point
    if(pp1.x()>pp2.x())swapPoint(pp1,pp2);
    else if(pp1.x()==pp2.x() && pp1.y()>pp2.y()) swapPoint(pp2,pp3);
    // ΔABC from left -> mid ->right
    left=pp1;
    mid=pp2;
    right=pp3;
    if (!isValid()) // three in line
    {
        circle_centerx=((left+right)/2).x();
        circle_centery=((left+right)/2).y();
        QPointF v=right-left;
        radius_square=(v.x()*v.x()+v.y()*v.y())/4;
        return;

    }

    // center: https://www.zhihu.com/question/39977986/answer/752410787, https://blog.csdn.net/MallowFlower/article/details/79919797
    //det(determinant) :https://baike.baidu.com/item/%E4%B8%89%E9%98%B6%E8%A1%8C%E5%88%97%E5%BC%8F/4466518?fr=aladdin

    //    已知三点坐标，求外接圆圆心坐标与半径。
    //    a=((y2-y1)*(y3*y3-y1*y1+x3*x3-x1*x1)-(y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1))/(2.0*((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1)));
    //    b=((x2-x1)*(x3*x3-x1*x1+y3*y3-y1*y1)-(x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1))/(2.0*((y3-y1)*(x2-x1)-(y2-y1)*(x3-x1)));
    //    r^2=(x1-a)*(x1-a)+(y1-b)*(y1-b);（此处为r的平方）
    double x1=pp1.x();
    double x2=pp2.x();
    double x3=pp3.x();
    double y1=pp1.y();
    double y2=pp2.y();
    double y3=pp3.y();
    circle_centerx=((y2-y1)*(y3*y3-y1*y1+x3*x3-x1*x1)-(y3-y1)*(y2*y2-y1*y1+x2*x2-x1*x1))/(2.0*((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1)));
    circle_centery=((x2-x1)*(x3*x3-x1*x1+y3*y3-y1*y1)-(x3-x1)*(x2*x2-x1*x1+y2*y2-y1*y1))/(2.0*((y3-y1)*(x2-x1)-(y2-y1)*(x3-x1)));
    radius_square=(x1-circle_centerx)*(x1-circle_centerx)+(y1-circle_centery)*(y1-circle_centery);
    radius=sqrt(radius_square);

    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x3 - x1;
    double by = y3 - y1;

    const double m = x2 * x2 - x1*x1 + y2 * y2 - y1 * y1;
    const double u = x3 * x3 + y3 *y3 - x1 * x1 - y1 * y1;
    const double s = 1. / (2 * (ax * by - ay * bx));

    circle_centerx = ((y3 - y1)*m + (y1 - y2)*u)*s;
    circle_centery = ((x1 - x3) * m + (x2 - x1)*u)*s;

    double dx = x1 - circle_centerx;
    double dy = y1 - circle_centery;
    radius_square = dx * dx + dy * dy;






}

void Triangle::setTriangle(Triangle &tri)
{
    left=tri.left;
    right=tri.right;
    mid=tri.mid;
    m_item=tri.m_item;
}


bool Triangle::isValid()
{
    QPointF v1= mid-left;
    QPointF v2=right-left;
//    if (v1.y()==0)
//        return v2.y()!=0;
//    if (v2.y()==0)
//        return v1.y()!=0;
    return v1.x()*v2.y()!=v1.y()*v2.x();
}

//该点是否在外接圆内
bool Triangle::CircleContain(const QPointF & p)const
{
    if (PointContain(p))
        return false;
    double d = ((p.x() - circle_centerx)*(p.x() - circle_centerx) + (p.y() - circle_centery)*(p.y() - circle_centery));
    return d < radius_square;
}

bool Triangle::TriangleContain(const QPointF &p)
{
//                  M(10,30)
//                    / \
//                   /   \
//                  /     \
//                 /   P   \      P'
//                /         \
//         L(0,0) ----------- R(20,0)

       /* Calculate area of triangle PBC */
       double A1 = areap3 (p.x(), p.y(), mid.x(), mid.y(), right.x(), right.y());

       /* Calculate area of triangle PAC */
       double A2 = areap3 (left.x(), left.y(), p.x(), p.y(), right.x(), right.y());

       /* Calculate area of triangle PAB */
       double A3 = areap3 (left.x(), left.y(), mid.x(), mid.y(), p.x(), p.y());

       /* Check if sum of A1, A2 and A3 is same as A */
       return (area() == A1 + A2 + A3);


//    QPointF v0=p-left;
//    QPointF v1=mid-left;
//    QPointF v2=right-left;

//    float dot00 = v0.x()*v0.x()+v0.y()*v0.y() ;
//    float dot01 = v0.x()*v1.x()+v0.y()*v1.y() ;
//    float dot02 = v0.x()*v2.x()+v0.y()*v2.y() ;
//    float dot11 = v1.x()*v1.x()+v1.y()*v1.y() ;
//    float dot12 = v1.x()*v2.x()+v1.y()*v2.y() ;

//    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

//    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
//    if ((u < 0) || (u > 1))
//        // if u out of range, return directly
//    {
//        return false ;
//    }

//    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
//    if ((v < 0 )||( v > 1))
//        // if v out of range, return directly
//    {
//        return false ;
//    }

//    return u + v <= 1 ;
}

bool Triangle::PointContain(const QPointF &p) const
{
    if (p.x()==left.x() && p.y()==left.y()) return true;
    if (p.x()==mid.x() && p.y()==mid.y()) return true;
    if (p.x()==right.x() && p.y()==right.y()) return true;
    return false;
}

Edge Triangle::EdgetoPoint(const QPointF &p)
{
    if (left == p)return getEdgemr();
    else if(mid==p) return getEdgelr();
    else if(right==p) return getEdgelm();
    return Edge();
}

bool Triangle::EdgeContain(const Edge &e)
//only for segment not contain straight line
{
    if (PointContain(e.p1) && PointContain(e.p2)) return 1;

}

QPointF Triangle::PointToEdge(const Edge &e)
{
    Edge ee1=Edge(left,mid);
    Edge ee2=Edge(mid,right);
    Edge ee3=Edge(left,right);

    if(ee1==e ) return right;
    if(ee2==e) return left;
    if(ee3==e) return mid;
}

int Triangle::getCrossEdge(const Edge &e, Edge *e1, Edge *e2)
{
    Edge ee1=Edge(left,mid);
    Edge ee2=Edge(mid,right);
    Edge ee3=Edge(left,right);
    int cnt=0;
    if(ee1==e || ee2==e ||ee3==e) return 0;
    if(ee1.crossEdge(e)==2)
    {
        e1->setEdge(ee1);
        cnt++;
    }
    if(ee2.crossEdge(e)==2)
    {
        if (cnt==1)
        {
            e2->setEdge(ee2);

        }
        else if(cnt==0)
            e1->setEdge(ee2);
        cnt++;
    }
    if(ee3.crossEdge(e)==2)
    {
        if (cnt==1)
        {
            e2->setEdge(ee3);

        }
        else if(cnt==0)
            e1->setEdge(ee3);
        cnt++;
    }

    return cnt;


}

void Triangle::getEdgeNextToPoint(const QPointF &p, Edge &e1, Edge &e2)
{
    if(p==left)
    {
        e1=getEdgelm();
        e2=getEdgelr();
    }
    else if(p==mid)
    {
        e1=getEdgelm();
        e2=getEdgemr();
    }
    else if(p==right);
    {
        e1=getEdgelr();
        e2=getEdgemr();
    }
}

bool Triangle::overlap(Triangle &tri)
{
    QPointF p1;
    if (!PointContain(tri.left))
    {
        p1=tri.left;
    }
    else if (!PointContain(tri.mid))
    {
        p1=tri.mid;
    }
    else if(!PointContain(tri.right))
    {
        p1=tri.right;
    }
    Edge overlap_e=tri.EdgetoPoint(p1);
    QPointF p2=PointToEdge(overlap_e);

    Edge cross_e=Edge(p1,p2);
    Edge* e1,*e2;
    int cross_n=getCrossEdge(cross_e,e1,e2);
    return cross_n==0;

}

Edge Triangle::commonEdge(Triangle &tri)
{
    //                    (10,30)     (30,30)
    //                     M---------- P
    //                    / \\        /
    //                   /   \\      /
    //                  /     \\    /
    //                 /       \\  /
    //                /         \\/
    //         L(0,0) ----------- R(20,0)
    // common edge = MR
    if (tri.EdgeContain(getEdgelm())) return getEdgelm();
    if (tri.EdgeContain(getEdgelr())) return getEdgelr();
    if (tri.EdgeContain(getEdgemr())) return getEdgemr();
    return Edge();

}

Edge Triangle::dialogEdge(Triangle &tri)
{
    //                    (10,30)     (30,30)
    //                     M---------- P
    //                    / \\        /
    //                   /   \\      /
    //                  /     \\    /
    //                 /       \\  /
    //                /         \\/
    //         L(0,0) ----------- R(20,0)
    // dialog edge = LR
    Edge e=commonEdge(tri);
    QPointF p1=tri.PointToEdge(e);
    QPointF p2=PointToEdge(e);
    return Edge(p1,p2);

}


void Triangle::setTriangleItem(QGraphicsPolygonItem *item)
{
    m_item=item;
}

QPointF Triangle::getcenter()
{
    QPointF temp=(left+right)/2;
    return (temp+mid)/2;
}

Edge Triangle::getEdgelm()
{
    return Edge(left,mid);
}
Edge Triangle::getEdgelr()
{
    return Edge(left,right);
}

Edge Triangle::getEdgemr()
{
    return Edge(mid,right);
}
Triangle::~Triangle()
{

}

int Triangle::neighbournum()
{
    int cnt=0;
    if (next_lm !=nullptr) cnt++;
    if (next_lr !=nullptr) cnt++;
    if (next_mr !=nullptr) cnt++;
    return cnt;
}

void Triangle::linkNext(Triangle *tri, const Edge &e)
{
    //m_neinum++;
    if (e==getEdgelm())
        next_lm=tri;
    else if(e==getEdgelr())
        next_lr=tri;
    else if(e==getEdgemr())
        next_mr=tri;

    return;

}

bool Triangle::linkNext(Triangle *tri)
{
    m_neinum++;
    if (tri==nullptr) return false;
    if (tri->PointContain(left))
    {
        if (tri->PointContain(mid))
        {
            if (next_lm!=nullptr)
                std::cout<<"next lm has been setted\n";
            next_lm=tri;
            return true;
        }
        else if (tri->PointContain(right))
        {
            if (next_lr!=nullptr)
                std::cout<<"next lr has been setted\n";
             next_lr=tri;
            return true;
        }
    }
    else if (tri->PointContain(mid))
    {
        if (tri->PointContain(right))
        {
            if (next_mr!=nullptr)
                std::cout<<"next mr has been setted\n";
            next_mr=tri;
            return true;
        }
    }
    if (3+3==6)
        return false;
}

Triangle *Triangle::nextTri()
{
    if ( next_lm)return next_lm;
    if ( next_lr)return next_lr;
    if (next_mr)return next_mr;
    return nullptr;
}

Triangle *Triangle::nextTri(Triangle *exclude1)
{
    if (exclude1 ==nullptr)return nullptr;
    if (exclude1 != next_lm && next_lm)return next_lm;
    if (exclude1 != next_lr && next_lr)return next_lr;
    if (exclude1 !=next_mr && next_mr)return next_mr;

    return nullptr;
}

Triangle *Triangle::nextTri(Triangle *exclude1, Triangle *exclude2)
{

    if (exclude1 ==nullptr)return nullptr;
    if (exclude1 != next_lm && exclude2 != next_lm)return next_lm;
    if (exclude1 != next_lr && exclude2 != next_lr)return next_lr;
    if (exclude1 !=next_mr && exclude2 != next_mr)return next_mr;

    return nullptr;
}

void Triangle::clearNext()
{
    next_lm=nullptr;
    next_lr=nullptr;
    next_mr=nullptr;
    m_neinum=0;
}

int Triangle::leafNeighbourNum()
{
    int leaf_nei=0;
    if (next_lm!=nullptr) leaf_nei+=next_lm->neighbournum()==1;
    if (next_lr!=nullptr) leaf_nei+=next_lr->neighbournum()==1;
    if (next_mr!=nullptr) leaf_nei+=next_mr->neighbournum()==1;
    return leaf_nei;

}

Triangle * Triangle::UpdataLink()
{
    Triangle * next=nullptr;
    Triangle * next_lmt=next_lm;
    Triangle * next_lrt=next_lr;
    Triangle * next_mrt=next_mr;
    clearNext();
    if (!linkNext(next_lmt) && (next_lmt!=nullptr)) next=next_lmt;
    if (!linkNext(next_lrt) && (next_lrt!=nullptr)) next=next_lrt;
    if (!linkNext(next_mrt) && (next_mrt!=nullptr)) next=next_mrt;

    return next;
}


double Triangle::area()
{
    if (!isValid()) return 0;
    double aarea=abs(0.5 * ((mid.x() - left.x()) * (right.y() -left.y()) - (right.x() - left.x()) * (mid.y() - left.y())));
    return aarea;
}

double Triangle::boudingRectArea()
{
    double dx=right.x()-left.x();
    double dy=bottom()-top();
    return abs(dx*dy);
}

double Triangle::top()
{
    double min_y=left.y();
    if (min_y>mid.y()) min_y=mid.y();
    if (min_y>right.y()) min_y=right.y();
    return min_y;

}

double Triangle::bottom()
{
    double max_y=left.y();
    if (max_y<mid.y()) max_y=mid.y();
    if (max_y<right.y()) max_y=right.y();
    return max_y;
}

void swapPoint(QPointF &p1, QPointF &p2)
{
    QPointF temp=p2;
    p2=p1;
    p1=temp;
}


bool Edge::operator<(const Edge &e)const
{
    //        int area2=2=e2.p2.y()*e2.p2.x()-e2.p1.y()*e2.p1.x();
    //        int area1=2=p2.y()*p2.x()-p1.y()*p1.x();
    //        if (area1-area2 !=0)
    //            return area1<area2;
    //        else
            double diffx1=p1.x()-e.p1.x();
            double diffx2=p2.x()-e.p2.x();
            double diffy1=p1.y()-e.p1.y();
            double diffy2=p2.y()-e.p2.y();

            // left point x less
            if (diffx1<0)
                return true;
            else if(diffx1>0)
                return false;
            //if left point x same y less
            else if (diffy1<0)
                return true;
            else if (diffy1>0)
                return false;
            //if left point same point
            else if(diffx2<0)
                return true;
            else if(diffx2>0)
                return false;
            else if(diffy2<0)
                return true;
            else if(diffy2>0)
                return false;
            return false;
}

//Edge &Edge::operator=(const Edge &t)
//{
//    p1=t.p1;
//    p2=t.p2;
//    return *this;
//}

Edge::Edge()
{
    p1=QPointF(0,0);
    p2=QPointF(0,0);
}

Edge::Edge(const Edge &e)
{
    p1=e.p1;
    p2=e.p2;
}

void Edge::setEdge(const Edge &e)
{
    p1=e.p1;
    p2=e.p2;
}

int Edge::crossEdge(const Edge &e)
{
    // no cross  and not in line return 0
    // no cross and four in line return 1

    // cross return 2
    // cross and same point   return 3
    // cross and four in line return 4


    QPointF p1q1=p1-e.p1;
    QPointF q2q1=e.p2-e.p1;
    QPointF p2q1=p2-e.p1;
    double crossl1=crossmultiplex(p1q1,q2q1);
    double crossr1=crossmultiplex(p2q1,q2q1);
    double crossmultiplex1=crossl1*crossr1;

    QPointF q1p1=e.p1-p1;
    QPointF p2p1=p2-p1;
    QPointF q2p1=e.p2-p1;
    double crossl2=crossmultiplex(q1p1,p2p1);
    double crossr2=crossmultiplex(q2p1,p2p1);
    double crossmultiplex2=crossl2*crossr2;

    /*
     *  <0 <0 cross and not three in line
     *  =0 =0 cross and two point at same point
     *  =0 >0 no cross and three inline
     *  =0 <0 cross and three in line
     *  >0 >0 no cross
     *  >0 <0 no cross
     */

    if (crossmultiplex1<0 && crossmultiplex2<0)
        return 2;
    else if(crossmultiplex1!=0 && crossmultiplex2!=0)
        return 0;
    else if((crossmultiplex1!=0)^(crossmultiplex2!=0))
    {
        if(crossmultiplex1<0 || crossmultiplex2<0)
            return 2;
        else
            return 0;
    }
    else if (crossl1==0 && crossl2==0 && crossr1==0 && crossr2==0)
    {
        // in line
        //no cross
        if (p1.x()>e.p2.x() || e.p1.x()>p2.x())
            return 1;
        // cross
        else return 4;
    }
    else return 3;
}

double Edge::length()
{
    QPointF p=p1-p2;
    return sqrt(p.x()*p.x()+p.y()+p.y());
}

bool Edge::operator ==(const Edge &e)const
{

    if (e.p1 == p1 && e.p2 == p2)return true;
    else if (e.p2 == p1 && e.p1 == p2)return true;
    else return false;

}


void Node::UpdateMaybeInDen()
{
    int valid=0;
    if (nodelist.length()>2 && mtri!=nullptr)
    {
        Triangle * tri_next1=mtri->nextTri();
        if (tri_next1->neighbournum()==1)
            valid++;

        Triangle * tri_next2=mtri->nextTri(tri_next1);
        if (tri_next2->neighbournum()==1)
            valid++;

        Triangle * tri_next3=mtri->nextTri(tri_next1,tri_next2);
        if (tri_next3->neighbournum()==1)
            valid++;

        if (valid!=1)
        {
            MaybeInDen=true;
        }
    }
}

Node::Node()
{

}

Node::Node(QPointF p)
{
    mp=p;
}

Node::Node(double x, double y)
{mp=QPointF(x,y);

}

Node::Node(Triangle *tri)
{
 mtri=tri;
 mp=tri->getcenter();
}

Node::~Node()
{
    if (nodelist.isEmpty()) return;
    for  (int i=0;i<nodelist.length();i++)
    {
        delete nodelist[i];
    }

}

int Node::numTraveled(QList<Node*> &travels)
{
    int cnt=0;
    foreach (auto nod1, nodelist) {
        foreach (auto nod2, traveledlist) {
            if (nod1==nod2)
            {
                cnt++;
                travels.append(nod1);
                break;
            }
        }
    }
    traveledlist.clear();
    foreach (auto nod, travels) {
        traveledlist.append(nod);
    }
    return cnt;

}

void Node::addNode(Node *node)
{
    nodelist.append(node);
    UpdateMaybeInDen();

}

bool Node::HasNode(Node *node)
{
    foreach (auto nod, nodelist) {
        if (nod==node)
            return true;
    }
    return false;
}

Node * Node::findNotOrdinay(Node * nod1, Node *nod2)
{
    // must be nordinary first

    if (isOrdianry())
    {
        for (int i = 0; i < nodelist.length(); ++i) {
            Node * tnod=nodelist.at(i);
            if (nod1==tnod)continue;
            if (tnod->isOrdianry())
                return tnod->findNotOrdinay(this);
            else if(tnod->isJunc())
            {
                tnod->traveledlist.append(this);
                return tnod;
            }
        }

    }
    else if(isJunc())
    {
        for (int i = 0; i < nodelist.length(); ++i) {
            Node * tnod=nodelist.at(i);
            if (nod1==tnod || nod2==tnod)continue;
            if (tnod->isOrdianry())
                return tnod->findNotOrdinay(this);
            else if(tnod->isJunc())
            {
                tnod->traveledlist.append(this);
                return tnod;
            }
        }
    }
    return nullptr;
}

bool Node::isOrdianry()
{
    UpdateMaybeInDen();
    return MaybeInDen==false;
//    int valid=0;
//    if (!isJunc()) return false;

//    if(nodelist.at(0)->isLeaf()) valid++;
//    if(nodelist.at(1)->isLeaf()) valid++;
//    if(nodelist.at(2)->isLeaf()) valid++;
//    return valid==1;
}

void Node::clear()
{
    nodelist.clear();
}


//Node *Node::nextNode()
//{
//    if(nodelist.isEmpty())return nullptr;
//    return nodelist.at(0);

//}

//Node * Node::nextNode(Node *exclude)
//{
//    for (int i=0;i<nodelist.length();i++)
//    {
//        Node *node=nodelist.at(i);
//        if (node->mtri!=exclude->mtri)
//            return node;
//    }
//    return nullptr;
//}

//Node *Node::nextNode(Node *exclude1, Node *exclude2)
//{
//    for (int i=0;i<nodelist.length();i++)
//    {
//        Node *node=nodelist.at(i);
//        if (node->mtri!=exclude1->mtri && node->mtri!=exclude2->mtri)
//            return node;
//    }
//    return nullptr;
//}

int Node::nextLeafNum()
{
    int cnt=0;
    if(isJunc())
    {
        Node * node1=nodelist.at(0);
        Node * node2=nodelist.at(1);
        Node * node3=nodelist.at(2);
        if (node1->isLeaf())cnt++;
        if (node2->isLeaf())cnt++;
        if (node3->isLeaf())cnt++;


    }
    if(isLinker())
    {
        Node * node1=nodelist.at(0);
        Node * node2=nodelist.at(1);

        if (node1->isLeaf())cnt++;
        if (node2->isLeaf())cnt++;



    }
    else if(isLeaf() && !nodelist.isEmpty())
    {
        Node * node1=nodelist.at(0);

        if (node1->isLeaf())cnt++;

    }
    return cnt;


}


double det(int n, double *Mat)
{
    if (n == 1)
        return Mat[0];
    double *subMat = new double[(n - 1)*(n - 1)];//创建n-1阶的代数余子式阵subMat
    int mov = 0;//判断行是否移动
    double sum = 0.0;//sum为行列式的值
    for (int Matrow = 0; Matrow<n; Matrow++) // Mat的行数把矩阵Mat(nn)赋值到subMat(n-1)
    {
        for (int subMatrow = 0; subMatrow<n - 1; subMatrow++)//把Mat阵第一列各元素的代数余子式存到subMat
        {
            mov = Matrow > subMatrow ? 0 : 1; //subMat中小于Matrow的行，同行赋值，等于的错过，大于的加一
            for (int j = 0; j<n - 1; j++)  //从Mat的第二列赋值到第n列
            {
                subMat[subMatrow*(n - 1) + j] = Mat[(subMatrow + mov)*n + j + 1];
            }
        }
        int flag = (Matrow % 2 == 0 ? 1 : -1);//因为列数为0，所以行数是偶数时候，代数余子式为1.
        sum += flag* Mat[Matrow*n] * det(n - 1, subMat);//Mat第一列各元素与其代数余子式积的和即为行列式
    }
    delete[]subMat;
    return sum;
}

double crossmultiplex(QPointF &p1, QPointF &p2)
{
    //cross multi a×b=（x1y2-x2y1）
    return  p1.x()*p2.y()-p1.y()*p2.x();

}

//void GetSplitBorders(QList<Triangle *> seg_triangles,QPolygonF poly);
void getLinkNode(QList<Triangle *> seg_triangles, QList<Node *> &nodes,QList<Node *> &juncnodes)
{
    //label trangle property : link degredd , and change triangle to node point
    int neighbournum=0;
    for (int i=0;i<seg_triangles.length();i++)
    {
       //leaf ==1/0, trinode ==3, link==2
        Triangle *tri =seg_triangles.at(i);
        neighbournum=tri->neighbournum();
        if(neighbournum==1 && (!tri->traveled))
        {

            Node *prenode=new Node(tri);
            nodes.append(prenode);

            tri->traveled=true;
            constructFromLeaf(tri->nextTri(), tri,prenode,nodes,juncnodes);


        }


        }



}

void constructFromLeaf(Triangle *tri_cur, Triangle *tri_pre,Node *prenode,QList<Node *> &leafnodes,QList<Node *> &juncnodes)
{
    int neighbournum=0;
    if(tri_cur ==nullptr) return;
    if(tri_cur->traveled)
    {
        if(tri_cur->mnod!=nullptr)
        {
            Node * curnode =tri_cur->mnod;
            if (!prenode->HasNode(curnode))
            {
                curnode->addNode(prenode);
                prenode->addNode(curnode);
            }
            else return;
        }
        else
            return;
    }

    Node *newnode=nullptr;
    Triangle *tri_next=nullptr;
    neighbournum=tri_cur->neighbournum();

    if (neighbournum==2)
    {
        tri_cur->traveled=true;
        tri_next=tri_cur->nextTri(tri_pre);
        constructFromLeaf(tri_next,tri_cur,prenode,leafnodes,juncnodes);

    }
    else if(neighbournum==1)
    {
        newnode=new Node(tri_cur);
        tri_cur->traveled=true;
        tri_cur->mnod=newnode;
        newnode->addNode(prenode);
        prenode->addNode(newnode);
        if (tri_cur->nextTri()->neighbournum()!=3) // neighbour triangle is not juction triangle
            leafnodes.append(newnode);
        else if(tri_cur->nextTri()->leafNeighbourNum()>1) // neighbour if juction, but more than one leaf
            leafnodes.append(newnode);
        return;
    }
    else if(neighbournum==3)
    {
        newnode=new Node(tri_cur);
        tri_cur->traveled=true;
        tri_cur->mnod=newnode;
        prenode->addNode(newnode);
        newnode->addNode(prenode);
        int valid=0;

        if (tri_pre->neighbournum()==1)
            valid++;

        tri_next=tri_cur->nextTri(tri_pre);
        if (tri_next->neighbournum()==1)
            valid++;
        constructFromLeaf(tri_next,tri_cur,newnode,leafnodes,juncnodes);

        tri_next=tri_cur->nextTri(tri_pre,tri_next);
        if (tri_next->neighbournum()==1)
            valid++;
        constructFromLeaf(tri_next,tri_cur,newnode,leafnodes,juncnodes);
        if (valid!=1)
        {
            newnode->MaybeInDen=true;
            juncnodes.append(newnode);
        }


    }


}
double areap3(double x1, double y1, double x2, double y2, double x3, double y3)
{
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}



Spine::Spine(QPolygonF a)
{
    poly=a;
}

void Spine::setpolygon(QPolygonF a)
{
poly=a;
}

bool Node::isLeaf()
{
    return mtri->neighbournum()<2;
}

bool Node::isJunc()
{
    return mtri->neighbournum()==3;
}

bool Node::isLinker()
{
    return mtri->neighbournum()==2;
}

Node *Node::nextNode()
{
    if (nodelist.isEmpty()) return nullptr;
    return nodelist.at(0);
}

Node *Node::nextNode(Node *exclude)
{
    if (nodelist.isEmpty()) return nullptr;
    foreach (auto nod, nodelist) {
        if (nod!=exclude) return nod;
    }
    return nullptr;
}

Node *Node::nextNode(Node *exclude1, Node *exclude2)
{
    if (nodelist.isEmpty()) return nullptr;
    foreach (auto nod, nodelist) {
        if (nod!=exclude1 && nod!=exclude2) return nod;
    }
    return nullptr;
}





