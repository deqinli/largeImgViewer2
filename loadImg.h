#ifndef LOADIMG_H
#define LOADIMG_H

#include <QObject>
#include <QThread>
class loadImg : public QObject
{
    Q_OBJECT
public:
    explicit loadImg(QObject *parent = nullptr);

signals:
public slots:


private:
    QThread t;

};

#endif // LOADIMG_H
