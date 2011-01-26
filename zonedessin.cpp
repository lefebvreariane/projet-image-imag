#include <zonedessin.h>

ZoneDessin::ZoneDessin(QWidget *parent) :
        QWidget(parent)
{
    resultLabel = new LabelImage;
    resultLabel->setBackgroundRole(QPalette::Base);
    resultLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    resultLabel->setScaledContents(true);
    //painter(&resultImage);

    image.load(":/images/logo.png");
    changer_image_sans_save(image);

    historique = new Historique;
    connect(historique,SIGNAL(changer_image(QImage)),this,SLOT(changer_image_sans_save(QImage)));

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
    historique->ajouter_image(image);
    emit enable_undo_redo();
}

void ZoneDessin::changer_image_sans_save(QImage img)
{
    image = img;
    init_affichage();
    afficher_image();
}


void ZoneDessin::init_affichage()
{
    image_affichages = image.copy();
}
