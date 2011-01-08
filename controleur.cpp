#include "controleur.h"
#include "zonedessin.h"
#include <QtGui>

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
}

void Controleur::clic_recu()
{

    switch (mode) {
    case AUCUN: {
            break;
        }
    case PIPETTE: {
            pipette(z->resultLabel->X0,z->resultLabel->Y0);
        }
    case GREY: {
            RGB_to_grey();
            break;
        }
    default: {

        }

    }

}
void Controleur::pipette(int x, int y)
{
    QColor pixel = z->image.pixel(x,y);
    /*
    qDebug()<<pixel.red();
    qDebug()<<pixel.green();
    qDebug()<<pixel.blue();
    qDebug()<<pixel.alpha();
*/
    emit afficher_pixel(pixel.red(), pixel.green(),pixel.blue());

}

void Controleur::RGB_to_grey()
{   //lit l'image qui correspond au LabelImage (z->resultLabel)
    //traite l'image
    //affiche l'image correspondante

    if(!z->image.isNull() && !z->image.allGray()){
        double greyD;
        int greyI,i,j;
        for (i=0 ; i<z->image.width() ; i++){
            for(j=0 ; j<z->image.height() ; j++){
                QColor c = z->image.pixel(i,j);
                greyD = c.red()*0.299 + c.green()*0.587 + c.blue()*0.114 + 0.5;
                greyI = (int) greyD;
                z->image.setPixel(i,j,qRgb(greyI,greyI,greyI));
            }
        }
        //z->image.setColorCount(256);
    }
    z->afficher_image();
}
