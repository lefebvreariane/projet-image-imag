#include "zonedessin.h"

#include <QtGui>
ZoneDessin::ZoneDessin(QWidget *parent) :
        QWidget(parent)
{
    resultLabel = new LabelImage;
    resultLabel->setBackgroundRole(QPalette::Base);
    resultLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    resultLabel->setScaledContents(true);
    //painter(&resultImage);

}

void ZoneDessin::afficher_image()
{
    resultLabel->setPixmap(QPixmap::fromImage(image));

    resultLabel->adjustSize();


}

void ZoneDessin::ecrire_image()
{

}
