#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPoint>
#include<QPointF>
#include <cmath>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QList>
class Node;
class Triangle;
struct Edge {
    QPointF p1;
    QPointF p2;
    Triangle * mtri;

    Edge ();
    Edge(QPointF pp1, QPointF pp2) {
        if (pp1.x() < pp2.x())
        {
            p1 = pp1;
            p2 = pp2;
        }
        else if(pp1.x() == pp2.x()&&pp1.y() < pp2.y() )
        {
            p1 = pp1;
            p2 = pp2;
        }

        else {
            p1 = pp2;
            p2 = pp1;
        }

    }
    Edge(const Edge &e);

    void setEdge(const Edge & e);
    int crossEdge(const Edge &e);
    double length();

    bool operator == (const Edge& e)const;

    bool operator<(const Edge& e)const;
    //Edge& operator=(const Edge& t);


};

class Triangle
{
public:
    Triangle();
    Triangle(QPointF pp1, QPointF pp2, QPointF pp3);
   Triangle(QPointF p, Edge e) :Triangle(p, e.p1, e.p2) {}
   Triangle(const QGraphicsPolygonItem &p);
   Triangle(const Triangle& t);
   Triangle& operator=(const Triangle& t);
   bool operator == (const Triangle& t)const;
   void setPoint(QPointF pp1, QPointF pp2, QPointF pp3);
   void setTriangle(Triangle & tri);

   //----------- judge triangle legal
    bool isValid();
   static bool isValid(const QPointF &p,const  Edge &e); //whether three point not in line

   //------- Judge some geomety struct relation in space
    bool CircleContain(const QPointF& p)const; // circumcircle include point?
    bool TriangleContain(const QPointF& p); //point whether in triangle
    bool PointContain(const QPointF &p)const; // point whether in three point
    Edge EdgetoPoint(const QPointF &p);
    bool EdgeContain(const Edge &e); //edge whether in three edge
    QPointF PointToEdge(const Edge &e); // return point which not in edge, edgecontain first, then do it
    int getCrossEdge(const Edge &e, Edge * e1, Edge * e2);//give which two edge was cross to give edge, return cross edge number
    void getEdgeNextToPoint(const QPointF &p,Edge &e1,Edge &e2);


    bool overlap(Triangle & tri);
    Edge commonEdge(Triangle & tri);
    Edge dialogEdge(Triangle &tri);
    QGraphicsPolygonItem* m_item;
    void setTriangleItem(QGraphicsPolygonItem* item);

    //void Draw(CDC* pDC);
    //void GetBoundingCircle(QPoint* center, int * radius);
    ~Triangle();
public:
    Node * mnod=nullptr;
    // trangle's three points
    QPointF left;
    QPointF mid;
    QPointF right;
    double circle_centerx;
    double circle_centery;
    double radius_square;
    double radius;
    //---------get triangle property
    double area();
    double boudingRectArea();
    double top();
    double bottom();
    QPointF getcenter();
    Edge getEdgelm();
    Edge getEdgelr();
    Edge getEdgemr();



    //--------used for traverse , Base on Graphic Index Search --------
    Triangle * next_lm=nullptr;//triangle neighbour to it
    Triangle *next_lr=nullptr;
    Triangle *next_mr=nullptr;
    int neighbournum();//0-3 , caculate how many triangle is its neighbour
    int m_neinum=0; // temp variable , used for debug
    void linkNext(Triangle * tri,const Edge &e); //bind neibour tri based on common edge
    bool linkNext(Triangle * tri);//bind neighbour tri, auto caculate common edge
    bool traveled=false;
    Triangle * nextTri();//return random neighbour tri
    Triangle * nextTri(Triangle * exclude1); //return random neighbour (not include exclude 1)
    Triangle * nextTri(Triangle *exclude1 , Triangle *exclude2);
    void clearNext();
    int leafNeighbourNum();//0-3 caculate how many leaf neighbour
    Triangle * UpdataLink();

    //






};

class Node : public QPointF
{
public:
    QPointF mp;
    Triangle * mtri=nullptr;
    Node * bound_node=nullptr;

    bool isInDen=false; //dennode
    bool MaybeInDen=false;
    void UpdateMaybeInDen();
    int spinecnt=0; // if spinecnt >1, maybeInDen =false

    QList<Node*> traveledlist;

    QList<Node*> nodelist;
    Node();
    Node(QPointF p);
    Node(double x,double y);
    Node (Triangle *tri);
    ~Node();
    int numTraveled(QList<Node*> &travels);
    void addNode(Node * node);
    bool HasNode(Node *node);

    Node *findNotOrdinay(Node * nod1, Node *nod2=nullptr);
    bool isOrdianry();
    void clear();

    //TODO
    bool isLeaf();
    bool isJunc();
    bool isLinker();
    Node *nextNode();
    Node * nextNode(Node * exclude);
    Node * nextNode(Node *exclude1 , Node *exclude2);

    int nextLeafNum();
};


class Spine
{
public:
    Edge constrain;
    QPolygonF poly;
    Spine();

    Spine(QPolygonF a);


    Spine(const Spine& spine):poly(spine.poly),constrain(spine.constrain){}
    void setpolygon(QPolygonF a);
};



void swapPoint(QPointF &p1,QPointF &p2);

using namespace std;
double areap3(double x1, double y1, double x2, double y2, double x3, double y3);
void getLinkNode(QList<Triangle *> seg_triangles, QList<Node *> &nodes,QList<Node *> &juncnodes);
void constructFromLeaf(Triangle *tri_cur, Triangle *tri_pre,Node *prenode,QList<Node *> &leafnodes,QList<Node *> &juncnodes);
double det(int n, double *Mat);
double crossmultiplex(QPointF &p1,QPointF &p2);
#endif // TRIANGLE_H
