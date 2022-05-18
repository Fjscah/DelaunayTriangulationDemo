#include "paintboard.h"

PaintBoard::PaintBoard(QWidget *parent)
    : QGraphicsView{parent}
{
    //QRect rect=geometry();
    scene=new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(geometry());
//    scene->setBackgroundBrush(QBrush(Qt::blue));
    pixitem=new QGraphicsPixmapItem();
    pixitem_backup=new QGraphicsPixmapItem();
    scene->addItem(pixitem);
    pixitem->setPos(-0.5,-0.5);
    pixitem_backup->setPos(-0.5,-0.5);



}



/* =↓=↓=↓=↓=↓=↓=  generate point and constrain segment edge  =↓=↓=↓=↓=↓=↓= */
void PaintBoard::GeneratePoints(int num)
{
    if (num<3)num=3;
    CleanAll();
    rect= geometry();
    scene->setSceneRect(geometry());
    QRectF r=scene->sceneRect();
//    QRect rect= QRect(r.topLeft().x(),r.topLeft().y(),r.width(),r.height()) ;
    std::cout<<"rect size : "<<rect.width()<<","<<rect.height()<<std::endl;
    std::cout<<"rect size : "<<r.width()<<","<<r.height()<<std::endl;
    srand(QTime::currentTime().msec());
    for (size_t i=0;i<num;i++)
    {
        QPointF *p=new QPointF(rand()%(rect.width()),rand()%(rect.height()));
        GraphicsPoint *item=new GraphicsPoint(-3,-3,6,6);
        item->setPos(*p);
        item->setBrush(QBrush(Qt::red));

        item->setFlags(QGraphicsItem::ItemIsFocusable);
//                |QGraphicsItem::ItemIsSelectable\
//                |QGraphicsItem::ItemIsMovable);
        scene->addItem(item);
        points.push_back(p);
        pointitems.push_back(item);
        //connect(item,SIGNAL(clicked_signal(GraphicsPoint*)),this,SLOT(on_item_clicked(GraphicsPoint*)));

    }

    CalcuBorder();



}

void PaintBoard::GeneratePointsFromPoly(const QPolygonF &poly)
{
    GraphicsPoint *item;
    for(int i=0;i<poly.length();i++)
    {
        item=new GraphicsPoint(-3,-3,6,6);
        item->setPos(poly.at(i));
        item->setBrush(QBrush(Qt::red));

        item->setFlags(QGraphicsItem::ItemIsFocusable
                       //|QGraphicsItem::ItemIsSelectable
                       |QGraphicsItem::ItemIsMovable);

        scene->addItem(item);
        pointitems.push_back(item);

    }




    for(int j=0;j<poly.length()-1;j++)
    {
        segments.push_back(Edge(poly.at(j),poly.at(j+1)));
    }
    segments.push_back(Edge(poly.at(poly.length()-1),poly.at(0)));
    QGraphicsPolygonItem * polyitem=new QGraphicsPolygonItem();
    polyitem->setPolygon(poly);
    QVector<QGraphicsPolygonItem*> polyitems;
    polyitems.append(polyitem);
    polyitemss.push_back(polyitems);

}
/* =↑=↑=↑=↑=↑=↑=  generate point and constrain segment edge  =↑=↑=↑=↑=↑=↑= */

/* =↓=↓=↓=↓=↓=↓=  Algorithm for Delaunay triangulation  =↓=↓=↓=↓=↓=↓= */
void PaintBoard::Bowyer()
{

    CleanResult();
    Delaunay::Delaunay_Points(points, triangles);
    updateTriangle();




}


void PaintBoard::ConstrainBowyer()
{

    CleanResult();
    Delaunay::Delaunay_Edges(points,triangles,segments);
    updateTriangle();

}

void PaintBoard::EachBowyer()
{
    CleanResult();
    QList<QPolygonF> polys;
    for (int i=0;i<segmentitems.length();i++)
    {
        QPolygonF poly=segmentitems.at(i)->polygon();
        polys.append(poly);
     }

    Delaunay::Delaunay_Contours(points, triangles,polys);
    updateTriangle();

}







/* =↑=↑=↑=↑=↑=↑=  Algorithm for Delaunay triangulation  =↑=↑=↑=↑=↑=↑= */


void PaintBoard::updateTriangle()
{
    float alpha=0.3;
    for (auto it = triangles.begin(); it != triangles.end();it++)
    {
        QPolygonF poly;
        poly.append((*it)->left);
        poly.append((*it)->mid);
        poly.append((*it)->right);

        QGraphicsPolygonItem *item=new QGraphicsPolygonItem;
        (*it)->setTriangleItem(item);
        //if(!(*it)->traveled)item->setBrush(QBrush(Qt::cyan));
        //if((*it)->neighbournum()==1)item->setBrush(QBrush(Qt::green));
        //if ((*it)->neighbournum()==3)
        //    item->setBrush(QBrush(Qt::red));
        //        else if ((*it)->neighbournum()==2)
        //            item->setBrush(QBrush(Qt::blue));
        //        else if ((*it)->neighbournum()==1)
        //            item->setBrush(QBrush(Qt::yellow));
        QPen pen = item->pen();
        pen.setWidth(1);
        pen.setCosmetic(true);
        item->setPen(pen);

        item->setFlags(//QGraphicsItem::ItemIsFocusable
                       QGraphicsItem::ItemIsSelectable
                       //|QGraphicsItem::ItemIsMovable
                       );

        item->setOpacity(alpha);
        item->setPolygon(poly);
        //item->setPos(0.5,0.5);
        triangleitems.push_back(item);
        scene->addItem(item);

    }
}


void PaintBoard::showTriangle(bool b)
{
    foreach (auto triangleitem, triangleitems) {
        scene->addItem(triangleitem);
        if(b) triangleitem->show();
        else triangleitem->hide();
    }

}

/*↑↑↑↑↑↑↑↑↑ Showing ↑↑↑↑↑↑↑↑↑*/



/* ↓↓↓↓↓↓↓  update/ clean /reset  buffer   ↓↓↓↓↓↓↓ */
void PaintBoard::CleanAll()// reset all data including random point generated
{
    // reset
    CleanResult();

    //List<*>
    triangleitems.clear();
    qDeleteAll(triangleitems.begin(), triangleitems.end());
    triangles.clear();

    segments.clear();
    qDeleteAll(segmentitems.begin(), segmentitems.end());
    segmentitems.clear();


    points.clear();
    pointitems.clear();
    qDeleteAll(pointitems.begin(), pointitems.end());

    scene->clear();
    if (curitem)
    {
        delete curitem;
        curitem=nullptr;
    }
    pixitem=nullptr;


}

void PaintBoard::CleanResult()// clean bowyer result data
{
    triangles.clear();
    for (int i=0;i<triangleitems.length();)
    {
        auto it=triangleitems.at(i);
        scene->removeItem(it);
        triangleitems.removeAt(i);
    }

    triangleitems.clear();



}

void PaintBoard::clearCurrentitem()
{
    if(curitem!=nullptr)
    {
        scene->removeItem(curitem);

        delete curitem;
        curitem =nullptr;
    }

}
void PaintBoard::ResetBorder()
{
    m_minX=std::numeric_limits<int>::max();
    m_maxX=std::numeric_limits<int>::min();
    m_minY=m_minX;
    m_maxY=m_maxX;
}

void PaintBoard::CalcuBorder()
{
    ResetBorder();
    int x,y;
    foreach (auto p, pointitems) {
        x=p->x();
        y=p->y();
        if (x > m_maxX)m_maxX = x;
        if (x < m_minX)m_minX = x;
        if (y> m_maxY)m_maxY = y;
        if (y < m_minY)m_minY = y;
    }
}
/*  ↑↑↑↑↑↑↑ update/ clean /reset  ↑↑↑↑↑↑↑ */

/* ↓↓↓↓↓↓↓ MSg Event process ↓↓↓↓↓↓↓ ↓*/
void PaintBoard::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void PaintBoard::keyPressEvent(QKeyEvent *event)
{


    QGraphicsView::keyPressEvent(event);
}

void PaintBoard::keyReleaseEvent(QKeyEvent *event)
{

}


void PaintBoard::mouseDoubleClickEvent(QMouseEvent *event)
{
    //enclosed it
    //add point to point item
    if (Is_draw && curitem!=nullptr)
    {
        QPolygonF poly=curitem->polygon();
        poly.pop_back();
        curitem->setPolygon(poly);
        GraphicsPoint *item;
        for(int i=0;i<poly.length();i++)
        {
            item=new GraphicsPoint(-3,-3,6,6);
            item->setPos(poly.at(i));
            item->setBrush(QBrush(Qt::red));

            item->setFlags(QGraphicsItem::ItemIsFocusable
                           //|QGraphicsItem::ItemIsSelectable
                           |QGraphicsItem::ItemIsMovable);

            scene->addItem(item);
            QPointF *p=new QPointF(poly.at(i));
            points.push_back(p);

            pointitems.push_back(item);


        }
        for(int j=0;j<poly.length()-1;j++)
        {
            segments.push_back(Edge(poly.at(j),poly.at(j+1)));
        }
        segments.push_back(Edge(poly.at(poly.length()-1),poly.at(0)));
        segmentitems.push_back(curitem);
        //scene->removeItem(curitem);
        curitem=nullptr;
    }


    //points.push_back(p);
}

void PaintBoard::mousePressEvent(QMouseEvent *event)
{
    firstpoint=mapToScene(event->pos());
    curpoint=mapToScene(event->pos());
    if(Is_draw && event->button()==Qt::LeftButton)
    {
        if (curitem==nullptr)
        {
            //create
            curitem =new QGraphicsPolygonItem ();
            QPolygonF poly= curitem->polygon();
            poly.append(firstpoint);
            poly.append(firstpoint);
            curitem->setPolygon(poly);
            //QBrush brush=QBrush(QColor(Qt::blue));
           //curitem->setBrush(brush);
            QPen pen=curitem->pen();
            pen.setWidth(3);
            pen.setColor(QColor(Qt::blue));
            curitem->setPen(pen);
            scene->addItem(curitem);

        }
        else
        {
            // append
            QPolygonF poly= curitem->polygon();
            poly.append(firstpoint);

            curitem->setPolygon(poly);
        }


    }
    QGraphicsView::mousePressEvent(event);
}

void PaintBoard::mouseMoveEvent(QMouseEvent *event)
{

    QPointF scene_p=mapToScene(event->pos());
    QPointF p = event->pos();

    emit sig_CurrentPos(p.x(),p.y());
    if(Is_draw && curitem!=nullptr)
    {
        curpoint=mapToScene(event->pos());
        QPolygonF poly=curitem->polygon();
        poly.removeLast();
        poly.append(curpoint);
        curitem->setPolygon(poly);
    }
QGraphicsView::mouseMoveEvent(event);
}

void PaintBoard::mouseReleaseEvent(QMouseEvent *event)
{
//    if (Is_draw && event->button()==Qt::LeftButton && curitem!=nullptr)
//    {
//        QPolygonF poly=curitem->polygon();
//        if(poly.length()>1)
//        {
//            QPointF endp=poly.at(poly.length()-1);
//            QPointF startp=poly.at(0);
//            double dx=endp.x()-startp.x();
//            double dy=endp.y()-startp.y();
//            double dis2 =dx*dx+dy*dy;
//            if (dis2<4)
//            {
//                //enclosed it
//                //add point to point item
//                GraphicsPoint *item;
//                for(int i=0;i<poly.length();i++)
//                {
//                    item=new GraphicsPoint(-3,-3,6,6);
//                    item->setPos(poly.at(i));
//                    item->setBrush(QBrush(Qt::red));

//                    item->setFlags(QGraphicsItem::ItemIsFocusable
//                                   //|QGraphicsItem::ItemIsSelectable
//                                   |QGraphicsItem::ItemIsMovable);
//                    scene->addItem(item);
//                    pointitems.push_back(item);
//                }


//                //points.push_back(p);
//                curitem=nullptr;
//            }

//        }
//    }
    QGraphicsView::mouseReleaseEvent(event);
}





/* ↑↑↑↑↑↑↑↑↑↑↑↑ MSg Event process↑ ↑↑↑↑↑↑↑↑↑↑↑↑↑ */


//left to right, up to down

bool compare2(const GraphicsPoint *p1, const GraphicsPoint *p2)
{
    if(p1->x()==p2->x())
        return p1->y()<p2->y();
    return p1->x()<p2->x();
}


