#include "MyGView.h"
#include <QDebug>
#include <iostream>


MyGView::MyGView(QWidget* parent):QGraphicsView(parent)
{
    //connect(this,SIGNAL(sendPosition(QPointF)),)

    bIsLeftButtonPressed = false;
    setStyleSheet("padding: 0px; border: 0px;");//无边框
    setMouseTracking(true);//跟踪鼠标位置
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖条
    nLevel = 1;
    nTileWidth = 1024;
    connect(this, &MyGView::levelChanged, this, &MyGView::deal_levelChanged_slot);
}

MyGView::~MyGView()
{

}

void MyGView::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        //m_pt = mapToScene(event->pos());
        //qDebug() << "mouse press position->scene:(" <<m_pt.x()<<","<<m_pt.y()<<")";
        QPoint pt(m_pt.x(),m_pt.y());

        emit mousepress_signal(pt);
        m_startPos = event->pos();
        bIsLeftButtonPressed = true;
        setCursor(Qt::ClosedHandCursor);
        //m_lastPointF = event->pos();
        //m_startScenePos.setX(this->scene()->sceneRect().x());
        //m_startScenePos.setY(this->scene()->sceneRect().y());
    }

    if (event->button() == Qt::RightButton)
    {
//        QList<QGraphicsItem*> _list = this->scene()->items();
//        for(QGraphicsItem* item:_list)
//        {

//            item->setPos(-nTileWidth/2,-nTileWidth/2);
//        }
        resetPos_signal(nLevel);
        qDebug() << "current level : " << nLevel;
    }

    QGraphicsView::mousePressEvent(event);

}

void MyGView::mouseMoveEvent(QMouseEvent *event)
{

    QPoint point = event->pos();
    emit mousemove_signal(point);



    if(bIsLeftButtonPressed)
    {

        QList<QGraphicsItem*> _list = this->scene()->items();
        int nSize = _list.size();
        m_endPos =event->pos();
        for (int nIndex=0; nIndex<_list.size(); nIndex++)
        {

            QPointF pt = _list[nIndex]->mapFromScene(event->pos()) - _list[nIndex]->mapFromScene(m_startPos);
            _list[nIndex]->moveBy(pt.x(),pt.y());

            qDebug() << " item->pos:: " << _list[nIndex]->pos();
        }
        m_startPos = event->pos();
        qDebug() << "-------------------------------------------------------";


        emit moveImg_signal(nLevel);
    }
    QGraphicsView::mouseMoveEvent(event);
    this->scene()->update();
}


void MyGView::wheelEvent(QWheelEvent *event)
{

    int nWheelDeltaValue = event->delta();

    int nTempLevel = getCurrentLevel();

    // 向上滚动，放大
    if(nWheelDeltaValue > 0)
    {
        nTempLevel += 1;
        if (nTempLevel >= 5)
        {
            nTempLevel = 5;
        }
    }
    else // 向下滚动，缩小
    {
        nTempLevel -= 1;
        if (nTempLevel <= 1)
        {
            nTempLevel = 1; // 等级最小为 1 。
        }
    }
    //setLevel(nTempLevel);

    if (nLevel <= 5)
    {
        //this->scene()->setSceneRect(-nTileWidth / nLevel, -nTileWidth / nLevel, nTileWidth * nLevel, nTileWidth * nLevel);
        //this->scene()->setSceneRect(-nWidth_GView/2, -nHeight_GView/2, nTileWidth * nLevel, nTileWidth * nLevel);

        QList<QGraphicsItem*> _list = this->scene()->items();
        for(QGraphicsItem* item:_list)
        {

            item->setPos(-nTileWidth/4,-nTileWidth/4);
        }
    }
    emit levelChanged(nTempLevel);
    update();
}

void MyGView::mouseReleaseEvent(QMouseEvent *event)
{
    if(bIsLeftButtonPressed)
    {
        bIsLeftButtonPressed = false;
        setCursor(Qt::CrossCursor);
        m_startPos = event->pos();
    }

    QGraphicsView::mouseReleaseEvent(event);
}

bool MyGView::setLevel(int nLevel)
{
    this->nLevel = nLevel;
    return true;
}

bool MyGView::setTileWidth(int nWidth)
{
    this->nTileWidth = nWidth;
    return true;
}

int MyGView::getCurrentLevel()
{
    return this->nLevel;
}

int MyGView::getTileWidth()
{
    return this->nTileWidth;
}

void MyGView::deal_levelChanged_slot(int nLevel)
{
    setLevel(nLevel);
    emit reloadImg_signal(nLevel);
}

void MyGView::setGViewSize(int nWidth_GView, int nHeight_GView)
{
    this->nWidth_GView = nWidth_GView;
    this->nHeight_GView = nHeight_GView;
}
