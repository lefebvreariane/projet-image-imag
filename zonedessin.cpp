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

    image.load(":/images/logo.png");
    afficher_image();

    //resultLabel->setPixmap(QPixmap(":/images/logo.png"));
    //resultLabel->adjustSize();
}

void ZoneDessin::afficher_image()
{
    resultLabel->setPixmap(QPixmap::fromImage(image));
    resultLabel->adjustSize();
}

void ZoneDessin::ecrire_image()
{

}

void ZoneDessin::changer_image(QImage img)
{
    image = img;
    afficher_image();
}
