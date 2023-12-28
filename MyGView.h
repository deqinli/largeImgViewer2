#ifndef MYGVIEW_H
#define MYGVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneWheelEvent>
#include <QHash>
#include <QString>
#include <QImage>


struct s_TilePosInfo
{
    int nRow;
    int nCol;
    s_TilePosInfo()
    {
        nRow = 0;
        nCol = 0;
    }
};

class MyGView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGView(QWidget* parent = 0);
    ~MyGView();
public:
    QPointF m_pt;
    QPointF m_lastPointF;
    bool bIsLeftButtonPressed;
    QPointF m_startPos;
    QPointF m_endPos;
    QPointF m_startScenePos;




    int getCurrentLevel();
    int getTileWidth();
    bool setLevel(int nLevel);
    bool setTileWidth(int nWidth);

    void setGViewSize(int nWidth_GView, int nHeight);
    int getGViewSize();




private:
    qreal scale_m; // 缩放值
    int nLevel;    // 当前缩放等级
    int nTileWidth;

    int nWidth_GView; // 主窗口的GView显示窗口的宽度
    int nHeight_GView; // 主窗口的GView显示窗口的高度




signals:
    void mousemove_signal(QPoint point);
    void mousepress_signal(QPoint point);
    void reloadImg_signal(int nLevel);
    void moveImg_signal(int nLevel);
    void levelChanged(int nLevel);
    void resetPos_signal(int nLevel);

private slots:
    void deal_levelChanged_slot(int nLevel);


protected:
//    void mousePressEvent(QMouseEvent* event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
//    void wheelEvent(QWheelEvent* event);
//    void mouseReleaseEvent(QMouseEvent* mouseEvent);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

};

#endif // MYGVIEW_H
