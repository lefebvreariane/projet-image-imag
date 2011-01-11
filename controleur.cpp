#include "controleur.h"
#include "zonedessin.h"
#include <QtGui>

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
    sX0 = -1;
    sX1 = -1;
    sY0 = -1;
    sY1 = -1;
    histogramme = new Histogramme (z->image);
}

void Controleur::reInitSelection()
{
    sX0 = -1;
    sX1 = -1;
    sY0 = -1;
    sY1 = -1;
}

int min(int i, int j)
{
    if(i>j)
        return j;
    else
        return i;
}

int max(int i, int j)
{
    if(i<j)
        return j;
    else
        return i;
}

void Controleur::clic_recu()
{

    switch (mode) {
    case SELECTION: {

            qDebug()<< "X0 = " << sX0;
            qDebug()<< "Y0 = " << sY0;
            qDebug()<< "X1 = " << sX1;
            qDebug()<< "Y1 = " << sY1;

            QPainter paint(&(z->image));
            if (sX0 == -1 || !(z->resultLabel->X0 > sX0 && z->resultLabel->X0 < sX0 + sX1 &&
                z->resultLabel->Y0 > sY0 && z->resultLabel->Y0 < sY0 + sY1))
            {
                sX0 = min(z->resultLabel->X0,z->resultLabel->X1);
                sY0 = min(z->resultLabel->Y0,z->resultLabel->Y1);
                sX1 = max(z->resultLabel->X0,z->resultLabel->X1) - sX0;
                sY1 = max(z->resultLabel->Y0,z->resultLabel->Y1) - sY0;
            }
            else
            {
                sX0 += z->resultLabel->X1 - z->resultLabel->X0;
                sY0 += z->resultLabel->Y1 - z->resultLabel->Y0;
            }
            paint.drawRect(sX0,sY0,sX1,sY1);
            z->afficher_image();
            break;
        }
    case PIPETTE: {
            pipette(z->resultLabel->X1,z->resultLabel->Y1);
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

    if(!z->image.isNull() && !z->image.allGray())
    {
        double greyD;
        int greyI,i,j;

        for (i=0 ; i<z->image.width() ; i++)
        {
            for(j=0 ; j<z->image.height() ; j++)
            {
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

void Controleur::appliquer_flou()
{
    QImage imFloue = z->image.copy(0,0,z->image.width(),z->image.height());
    int i,j,k,l;
    int compteur;
    double r,g,b; // composantes de la nouvelle couleur
    int tailleFiltre = 7;
    int distPixel = (int) tailleFiltre/2;

    // On applique le filtre de moyenne (à 1) de taille 3 par default
    for(i=0 ; i<=imFloue.width()-1 ; i++)
    {
        for(j=0 ; j<=imFloue.height()-1 ; j++)
        {
            // On fait la somme des taille^2 pixels (des taille^2 qui
            // entourent le point sur lequel on est entrain d'appliquer
            // le filtre et lui même, seulement s'ils ne sont pas en
			// dehors de la zone image)
            r = g = b = 0;
            compteur = tailleFiltre*tailleFiltre;
            for (k=-distPixel ; k<=distPixel ; k++)
            {
                for(l=-distPixel ; l<=distPixel ; l++)
                {
                    if ((i+k>=0) && (i+k<=z->image.width()-1) &&
                        (j+l>=0) && (j+l<=z->image.height()-1))
                    {
                        r += qRed(z->image.pixel(i+k,j+l));
                        g += qGreen(z->image.pixel(i+k,j+l));
                        b += qBlue(z->image.pixel(i+k,j+l));
                    }
                    else compteur--;
                }
            }
            // Puis on divise la somme par le nombre d'additions effectuees
            if (compteur != 0)
            {
                r = r/(compteur);
                g = g/(compteur);
                b = b/(compteur);
              imFloue.setPixel(i,j,qRgb((int)r,(int)g,(int)b));
            }
        }
    }
    //qDebug()<< "image originale un pixel:" << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qBlue(z->image.pixel(distPixel,distPixel));
    z->image = imFloue;
    //qDebug()<< "image floutee un pixel:" << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qBlue(z->image.pixel(distPixel,distPixel));

    z->afficher_image();
}


QImage Controleur::decouper()
{
    int x = 0, y = 0;
    //int largeur = max(z->resultLabel->X0, z->resultLabel->X1) - min(z->resultLabel->X0, z->resultLabel->X1);
    //int hauteur = max(z->resultLabel->Y0, z->resultLabel->Y1) - min(z->resultLabel->Y0, z->resultLabel->Y1);
    //QImage resImage(largeur,hauteur,z->image.format());
    QImage resImage(sX1,sY1,z->image.format());

    if(z->resultLabel->X0 == z->resultLabel->X1 || z->resultLabel->Y0 == z->resultLabel->Y1)
        resImage = z->image;
    else {
        for(int i=sX0; i<sX0 + sX1; i++) {
            for(int j=sY0; j<sY0 + sY1; j++) {
                if (i < 0 || i > z->image.width() || j < 0 || j > z->image.height())
                    resImage.setPixel(QPoint(x,y++),qRgba(255,255,255,0));
                else
                    resImage.setPixel(QPoint(x,y++),z->image.pixel(i,j));
            }
            x++;
            y = 0;
        }
    }

    return resImage;
}

void Controleur::afficher_histogrammes()
{
    histogramme->show();
}
