#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPointF>
#include <QDebug>
#include <thread>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene;
    GView = new MyGView();
    int nWidth = ui->graphicsView->width();
    int nHeight = ui->graphicsView->height();

    m_scene->setSceneRect(QRect(-ui->graphicsView->width()/2.0,-ui->graphicsView->height()/2.0,nWidth,nHeight));
    ui->graphicsView->setScene(m_scene);

    GView->setGViewSize(nWidth,nHeight);
    GView->setLevel(1);
    //QString qstrTileSample = "../TileMap/LEVEL_1/R000000/C000000.tif";
//    QImage img(qstrTileSample);
//    if(!_memoryCache.contains(qstrTileSample))
//    {
//        _memoryCache.insert(qstrTileSample,&img);
//    }

    updateScene(GView->getCurrentLevel());



    // 设置光标为十字
    ui->graphicsView->setCursor(Qt::CrossCursor);

    // 启用鼠标跟踪器，即使没有按下鼠标，也会接收鼠标移动事件
    //ui->graphicsView->setMouseTracking(true);

    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    //
    //ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    connect(ui->graphicsView, &MyGView::mousemove_signal,this,&MainWindow::deal_mousemove_signal);
    //connect(ui->graphicsView, &MyGView::mousepress_signal,this,&MainWindow::deal_mousePress_signal);
    connect(ui->graphicsView, &MyGView::reloadImg_signal, this, &MainWindow::deal_reloadImg_signal);
    //connect(ui->graphicsView, &MyGView::moveImg_signal, this, &MainWindow::deal_moveImg_signal);
    connect(ui->graphicsView,&MyGView::resetPos_signal,this,&MainWindow::deal_resetPos);


    ui->graphicsView->show();
    auto m_pDeskdop = QApplication::desktop();
    move((m_pDeskdop->width() - this->width())/2, (m_pDeskdop->height() - this->height())/2);

}

MainWindow::~MainWindow()
{
    QHash<QString, QImage*>::iterator iter = _memoryCache.begin();
    for (; iter != _memoryCache.end(); iter++)
    {
        QString qstrName = iter.key();
        QImage* img = iter.value();
        delete img;
        img = 0;
    }
    _memoryCache.clear();
    delete ui;
}

QImage MainWindow::Mat2QImage(const cv::Mat &mat)
{
    switch (mat.type())
    {
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image;
    }
    case CV_8UC3:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1:
    {
        QVector<QRgb> sColorTable;
        if (sColorTable.isEmpty())
        {
            for (int i=0; i < 256; i++)
            {
                sColorTable.push_back(qRgb(i, i, i));
            }
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(sColorTable);
        return image;
    }
    default:
        qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
        break;
    }
    return QImage();
}

QPixmap MainWindow::Mat2QPixmap(const cv::Mat &mat)
{
    return QPixmap::fromImage(Mat2QImage(mat));
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
//        m_ptf = mapToGlobal( event->pos());
        //GView->mapToScene(
//        qDebug()<<pt;
        //getPosition();
        //m_ptf = GView->mapToScene(event->pos());

        //qDebug() << m_ptf;
    }
}

void MainWindow::deal_mousemove_signal(QPoint point)
{
    // 显示view下的坐标
    ui->viewLineEdit->setText(QString::asprintf("(%d, %d)",point.x(),point.y()));
    //ui->

    // 转换到scene坐标
    QPointF pointScene = ui->graphicsView->mapToScene(point);
    ui->sceneLineEdit->setText(QString::asprintf("(%.0f,%.0f)",pointScene.x(),pointScene.y()));

}

void MainWindow::deal_mousePress_signal(QPoint point)
{
    m_ptf = point;
    //qDebug() << m_ptf;
    //qDebug() << "mainwindow:(" <<m_ptf.x()<<","<<m_ptf.y()<<")";
    //HTuple hv_worldPoint_x,hv_worldPoint_y;
    //AffineTransPoint2d(hv_hom_mat,m_ptf.x(),m_ptf.y(),&hv_worldPoint_x,&hv_worldPoint_y);
    //qDebug() << "world coordinate:(" << hv_worldPoint_x[0].D() << "," << hv_worldPoint_y[0].D() << ")";
    //double delta_x = hv_worldPoint_x[0].D() - qPoint_Worldcenter.x();
    //double delta_y = hv_worldPoint_y[0].D() - qPoint_Worldcenter.y();
    //qDebug() << "△x:(" << delta_x << "), △y:(" << delta_y << ")";
}

void MainWindow::deal_reloadImg_signal(int nLevel)
{
    GView->setLevel(nLevel);
    QHash<QString, QImage*>::iterator iter = _memoryCache.begin();
    int _nSize = _memoryCache.size();
    for (; iter != _memoryCache.end(); iter++)
    {
        QString qstrName = iter.key();
        QImage* img = iter.value();
        delete img;
        img = 0;
    }
    _memoryCache.clear();
    int nTileWidth = GView->getTileWidth();
    //m_scene->setSceneRect(QRect(-ui->graphicsView->width()/2.0,-ui->graphicsView->height()/2.0,nTileWidth*pow(2,(nLevel-1)),nTileWidth*pow(2,(nLevel-1))));
    updateScene(nLevel);
    //deal_moveImg_signal(nLevel);
}

void MainWindow::deal_moveImg_signal(int nLevel)
{
    updateScene(nLevel);
}

void MainWindow::updateScene(int nLevel)
{
    QHash<QString, QImage*>::const_iterator iter = _memoryCache.constBegin();
    QStringList fileNameList;
    for (; iter != _memoryCache.constEnd(); iter++)
    {
        QString fileName = iter.key();
        s_TilePosInfo tileInfo;
        parseFileName(fileName, tileInfo);
        bool bFlag = bIsTileInView(tileInfo.nRow, tileInfo.nCol);
        if (!bFlag)
        {
            // 待删除的文件名
            fileNameList.append(fileName);
        }
    }

    for (QString filename : fileNameList)
    {
        delete _memoryCache.value(filename);
        _memoryCache.remove(filename);
    }


    m_scene->clear();

    if (nLevel <= 5)
    {
        QGraphicsItemGroup* itemGroup = new QGraphicsItemGroup();
        std::list<std::thread> threadList;
        for (int i = 0; i < 16; i++)
        {
            threadList.push_back(std::thread(&MainWindow::loadImage, this, nLevel));
        }
        //loadImage(nLevel);

        for (std::thread& thre : threadList)
        {
            thre.join();
            //thre.detach();
        }

        setItems(itemGroup);
        m_scene->addItem(itemGroup);

    }
}


void MainWindow::parseFileName(const QString filePath, s_TilePosInfo& tileInfo)
{
    QStringList strList = filePath.split(".tif");
    QStringList strList2 = strList[0].split("/");
    QString strCol = strList2[strList2.size() - 1];
    QString strRow = strList2[strList2.size() - 2];

    QStringList strColList = strCol.split("C");
    tileInfo.nCol = strColList[strColList.size() - 1].toInt();
    QStringList strRowList = strRow.split("R");
    tileInfo.nRow = strRowList[strRowList.size() - 1].toInt();

    //tileInfo.nRow = strList2[strList2.size() -2].split('R')[-1].toInt();
    int a = 0;
}

bool MainWindow::bIsTileInView(const int nRow, const int nCol)
{
    // 判断item的中心点坐标是否在view的视野内
    //int padding =1024 * pow(2,GView->getCurrentLevel()-1) /*GView->getTileWidth()*/;
    int padding =1024 /*GView->getTileWidth()*/;

    QPoint ptItem(GView->getTileWidth() * (nCol - 1)+GView->getTileWidth()/2 , GView->getTileWidth() * (nRow - 1)+GView->getTileWidth()/2);
    //QPoint pView = ui->graphicsView->mapFromScene(ptItem);
    QPoint pView = ptItem;
    QPoint leftTop(pView.x() - padding, pView.y() - padding);
    QPoint leftBottom(pView.x() - padding, pView.y() + padding);
    QPoint rightTop(pView.x() + padding, pView.y() - padding);
    QPoint rightBottom(pView.x() + padding, pView.y() + padding);


//    bool flag1 = ui->graphicsView->rect().contains(leftTop);
//    bool flag2 = ui->graphicsView->rect().contains(leftBottom);
//    bool flag3 = ui->graphicsView->rect().contains(rightTop);
//    bool flag4 = ui->graphicsView->rect().contains(rightBottom);

    bool flag1 = rightBottom.x() < ui->graphicsView->rect().bottomLeft().x();
    bool flag2 = leftBottom.x() > ui->graphicsView->rect().bottomRight().x();
    bool flag3 = rightBottom.y() < ui->graphicsView->rect().topRight().y();
    bool flag4 = leftTop.y() > ui->graphicsView->rect().bottomRight().y();
//    qDebug("ui->graphicsView->rect().topLeft().x() = %d", ui->graphicsView->rect().topLeft().x());
//    qDebug("ui->graphicsView->rect().topleft().y() = %d", ui->graphicsView->rect().topLeft().y());
//    qDebug("ui->graphicsView->rect().bottomRight().x() = %d", ui->graphicsView->rect().bottomRight().x());
//    qDebug("ui->graphicsView->rect().bottomRight().y() = %d", ui->graphicsView->rect().bottomRight().y());
    //return (flag);

    return (!(flag1||flag2||flag3||flag4));
}

void MainWindow::setItems(QGraphicsItemGroup* itemGroup)
{
    s_TilePosInfo tilInfo;
    QHash<QString, QImage*>::const_iterator iter = _memoryCache.constBegin();
    for (; iter != _memoryCache.constEnd(); iter++)
    {
        parseFileName(iter.key(), tileInfo);
        int _nTileWidth = GView->getTileWidth();

        QPixmap pixmap00 = QPixmap::fromImage(*iter.value());
        QPainter painter(&pixmap00);
        QPen pen;
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawText(QPoint(pixmap00.width() / 2, pixmap00.height() / 2), "(" + QString::number(tileInfo.nRow) + "," + QString::number(tileInfo.nCol) + ")");

        QGraphicsPixmapItem* pixItem00 = new QGraphicsPixmapItem(pixmap00);

        QPointF ptTempPos = QPointF(_nTileWidth * (tileInfo.nCol - 1), _nTileWidth * (tileInfo.nRow - 1));



        if(GView->getCurrentLevel() == 1)
        {
            ptTempPos = QPointF(-_nTileWidth/2, -_nTileWidth/2);
        }

        pixItem00->setPos(ptTempPos);

        QPointF ptitemScene = pixItem00->sceneBoundingRect().center();
        QPointF ptView = ui->graphicsView->mapFromScene(pixItem00->sceneBoundingRect().center());
        //std::cout << "pixItem00->boundingRect().center() :=> (" << ptitemScene.x() << "," << ptitemScene.y() << "), viewPt :=> (" << ptView.x() << "," << ptView.y() << ")" << std::endl;

        itemGroup->addToGroup(pixItem00);
    }


}


void MainWindow::loadImage(int nLevel)
{
    int nTiles_horizentol = pow(2, nLevel - 1);

    std::lock_guard<std::mutex> lockGuard(lock_memoryCache);

    for (int nRow = 0; nRow < nTiles_horizentol; nRow++)
    {
        for (int nCol = 0; nCol < nTiles_horizentol; nCol++)
        {
            // 判断item的中心点坐标是否在view的视野内
            bool bFlag = bIsTileInView(nRow, nCol);
            //QString qstrImg = QString("./TileMap/LEVEL_%1/R%2/C%3.JPG").arg(nLevel).arg(nRow, 6, 10, QLatin1Char('0')).arg(nCol, 6, 10, QLatin1Char('0'));
            QString qstrImg = QString("../TileMap/LEVEL_%1/R%2/C%3.tif").arg(nLevel).arg(nRow, 6, 10, QLatin1Char('0')).arg(nCol, 6, 10, QLatin1Char('0'));
            if (1)
            {
                if (!_memoryCache.contains(qstrImg))
                {
                    QImage* image = new QImage(qstrImg);
                    //pixmap00 = QPixmap::fromImage(*image);
                    _memoryCache.insert(qstrImg, image);
                }
            }
            else
            {
                if (_memoryCache.contains(qstrImg))
                {
                    QImage* img = _memoryCache.value(qstrImg);
                    delete img;
                    img = 0;
                    _memoryCache.remove(qstrImg);
                }
            }
        }
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(QEvent::WindowStateChange == event->type())
    {
          QWindowStateChangeEvent * stateEvent = dynamic_cast<QWindowStateChangeEvent*>(event);
          if(Q_NULLPTR != stateEvent)
          {
              if(this->isMaximized())
              {
                  //qDebug("最大化");
                  GView->setGViewSize(ui->graphicsView->width(),ui->graphicsView->height());
                  m_scene->setSceneRect(QRect(-ui->graphicsView->width()/2.0,-ui->graphicsView->height()/2.0,
                                            ui->graphicsView->width(),ui->graphicsView->height()));
                  int nLv = GView->getCurrentLevel();
                  qDebug() << "currentLevel = " << nLv;
                  updateScene(GView->getCurrentLevel());
              }
          }
      }
      QMainWindow::changeEvent(event);
}

void MainWindow::deal_resetPos(int nLevel)
{
    GView->setLevel(nLevel);
    updateScene(nLevel);
}
