#include "loadImg.h"

loadImg::loadImg(QObject *parent) : QObject(parent)
{
    moveToThread(&t);
    t.start();

}
