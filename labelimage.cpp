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
    qDebug()<<"PRESS";
    QPoint pos = event->pos();
    X0= pos.x();
    Y0= pos.y();



}


void LabelImage::mouseReleaseEvent(QMouseEvent *event)

{
    qDebug()<<"RELEASE";
    QPoint pos = event->pos();
    X1= pos.x();
    Y1= pos.y();

    if (X1 <0)
        X1 = 0;
    else if(X1 > this->width())
        X1 = this->width()-1;

    if (Y1 <0)
        Y1 = 0;
    else if(Y1 > this->height())
        Y1 = this->height()-1;


    emit clic();
}
