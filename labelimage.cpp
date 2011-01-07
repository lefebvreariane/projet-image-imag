#include "labelimage.h"
#include <QtGui>


LabelImage::LabelImage()
{
    X0=-1;
    X1=-1;
    Y0=-1;
    Y1=-1;
}

void LabelImage::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    X0= pos.x();
    Y0= pos.y();

    emit clic();
}
