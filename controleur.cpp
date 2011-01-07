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
