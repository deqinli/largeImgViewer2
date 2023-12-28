#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QEvent>
#include <QObject>
#include <QPointF>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QPixmap>
#include <opencv2/aruco/charuco.hpp>
#include "MyGView.h"


#include <mutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage Mat2QImage(const cv::Mat &mat);
    QPixmap Mat2QPixmap(const cv::Mat &mat);
    void loadImage(int nLevel);

private:
    Ui::MainWindow *ui;
    MyGView* GView;
    s_TilePosInfo tileInfo;
    QPointF m_ptf;

    cv::Mat srcImg;
    QGraphicsScene *m_scene;

    QHash<QString, QImage*> _memoryCache;
    std::mutex lock_memoryCache;



    void mousePressEvent(QMouseEvent* event);
    void parseFileName(const QString filePath, s_TilePosInfo& tileInfo);
    bool bIsTileInView(const int nRow, const int nCol);
    void setItems(QGraphicsItemGroup* itemGroup);
    void updateScene(int nLevel);

private slots:
    void deal_mousemove_signal(QPoint point);
    void deal_mousePress_signal(QPoint point);
    void deal_reloadImg_signal(int nLevel);
    void deal_moveImg_signal(int nLevel);
    void deal_resetPos(int nLevel);

protected:
    void changeEvent(QEvent* event);
};
#endif // MAINWINDOW_H
