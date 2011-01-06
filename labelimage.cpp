#include "labelimage.h"
#include <QtGui>



void LabelImage::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    qDebug()<<pos.x();
    qDebug()<<pos.y();

}
