#include "zonedessin.h"

ZoneDessin::ZoneDessin(QWidget *parent) :
        QWidget(parent)
{
    resultLabel = new LabelImage;
    resultLabel->setBackgroundRole(QPalette::Base);
    resultLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    resultLabel->setScaledContents(true);
    //painter(&resultImage);

    image.load(":/images/logo.png");
    changer_image(image);
}

void ZoneDessin::afficher_image()
{
    resultLabel->setPixmap(QPixmap::fromImage(image_affichages));
    resultLabel->adjustSize();

}

void ZoneDessin::ecrire_image()
{

}

void ZoneDessin::changer_image(QImage img)
{
    image = img;
    init_affichage();
    afficher_image();
}

void ZoneDessin::init_affichage()
{
    image_affichages = image.copy();
}
