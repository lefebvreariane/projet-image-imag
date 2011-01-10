#include "controleur.h"
#include "zonedessin.h"
#include <QtGui>

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
    histogramme = new Histogramme (z->image);
}

void Controleur::clic_recu()
{

    switch (mode) {

    case SELECTION: {
            qDebug()<< "X0 = " << z->resultLabel->X0;
            qDebug()<< "Y0 = " << z->resultLabel->Y0;
            qDebug()<< "X1 = " << z->resultLabel->X1;
            qDebug()<< "Y1 = " << z->resultLabel->Y1;
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

QImage Controleur::flouter_coins(QImage imIn, QImage imOut, int distPixel, int tailleFiltre)
{

    for (int n=0 ; n<distPixel ; n++)
    {
        double r,g,b;
        int k,l;
        // flou sur coin Haut/Gauche
        r = g = b = 0;
        for (k=0 ; k<=distPixel+n ; k++)
            for(l=0 ; l<=distPixel+n ; l++)
            {
            r += qRed(z->image.pixel(k,l));
            g += qGreen(z->image.pixel(k,l));
            b += qBlue(z->image.pixel(k,l));
        }
        r = r/(tailleFiltre*tailleFiltre);
        g = g/(tailleFiltre*tailleFiltre);
        b = b/(tailleFiltre*tailleFiltre);
        imOut.setPixel(n,n,qRgb((int)r,(int)g,(int)b));

        // flou sur coin Bas/Droite
        r = g = b = 0;
        for (k=imIn.width() ; k>=imIn.width()-distPixel-n ; k--)
            for(l=imIn.height() ; l>=imIn.height()-distPixel-n ; l--)
            {
            r += qRed(z->image.pixel(k,l));
            g += qGreen(z->image.pixel(k,l));
            b += qBlue(z->image.pixel(k,l));
        }
        r = r/(tailleFiltre*tailleFiltre);
        g = g/(tailleFiltre*tailleFiltre);
        b = b/(tailleFiltre*tailleFiltre);
        imOut.setPixel(imIn.width()-n,imIn.height()-n,qRgb((int)r,(int)g,(int)b));

        // flou sur coin Haut/Droite
        r = g = b = 0;
        for (k=imIn.width() ; k>=imIn.width()-distPixel-n ; k--)
            for(l=0 ; l<=distPixel+n ; l++)
            {
            r += qRed(z->image.pixel(k,l));
            g += qGreen(z->image.pixel(k,l));
            b += qBlue(z->image.pixel(k,l));
        }
        r = r/(tailleFiltre*tailleFiltre);
        g = g/(tailleFiltre*tailleFiltre);
        b = b/(tailleFiltre*tailleFiltre);
        imOut.setPixel(imIn.width()-n,n,qRgb((int)r,(int)g,(int)b));

        // flou sur coin Bas/Gauche
        r = g = b = 0;
        for (k=0 ; k<=distPixel+n ; k++)
            for(l=imIn.height() ; l>=imIn.height()-distPixel-n ; l++)
            {
            r += qRed(z->image.pixel(k,l));
            g += qGreen(z->image.pixel(k,l));
            b += qBlue(z->image.pixel(k,l));
        }
        // Puis on divise la somme par la taille du filtre au carré
        r = r/(tailleFiltre*tailleFiltre);
        g = g/(tailleFiltre*tailleFiltre);
        b = b/(tailleFiltre*tailleFiltre);
        imOut.setPixel(n,imIn.height()-n,qRgb((int)r,(int)g,(int)b));
    }
    return imOut;
}

QImage Controleur::flouter_bords(QImage imIn, QImage imOut, int distPixel, int tailleFiltre)
{
    return imOut;
}

void Controleur::appliquer_flou()
{
    QImage imFloue = z->image.copy(0,0,z->image.width(),z->image.height());
    int i,j,k,l;
    double r,g,b; // composantes de la nouvelle couleur
    int tailleFiltre = 3;
    int distPixel = (int) tailleFiltre/2;

    // On applique le filtre de moyenne (à 1) de taille 3 par default
    for(i=distPixel ; i<imFloue.width()-distPixel-1 ; i++)
    {
        for(j=distPixel ; j<imFloue.height()-distPixel-1 ; j++)
        {
            // On fait la somme des taille^2 pixels (des taille^2 qui
            // entourent le point sur lequel on est entrain d'appliquer
            // le filtre et lui même)
            r = g = b = 0;
            for (k=-distPixel ; k<=distPixel ; k++)
            {
                for(l=-distPixel ; l<=distPixel ; l++)
                {
                    r += qRed(z->image.pixel(i+k,j+l));
                    g += qGreen(z->image.pixel(i+k,j+l));
                    b += qBlue(z->image.pixel(i+k,j+l));
                }
            }
            // Puis on divise la somme par la taille du filtre au carré
            r = r/(tailleFiltre*tailleFiltre);
            g = g/(tailleFiltre*tailleFiltre);
            b = b/(tailleFiltre*tailleFiltre);
            imFloue.setPixel(i,j,qRgb((int)r,(int)g,(int)b));
        }
    }
    //imFloue = this->flouter_coins(z->image, imFloue, distPixel, tailleFiltre);
    //qDebug()<< "image originale un pixel:" << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qBlue(z->image.pixel(distPixel,distPixel));
    z->image = imFloue;
    //qDebug()<< "image floutee un pixel:" << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qRed(z->image.pixel(distPixel,distPixel)) << " ; " << qBlue(z->image.pixel(distPixel,distPixel));

    z->afficher_image();
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

QImage Controleur::decouper()
{
    int x = 0, y = 0;
    int largeur = max(z->resultLabel->X0, z->resultLabel->X1) - min(z->resultLabel->X0, z->resultLabel->X1);
    int hauteur = max(z->resultLabel->Y0, z->resultLabel->Y1) - min(z->resultLabel->Y0, z->resultLabel->Y1);
    QImage resImage(largeur,hauteur,z->image.format());

    if(z->resultLabel->X0 == z->resultLabel->X1 || z->resultLabel->Y0 == z->resultLabel->Y1)
        resImage = z->image;
    else {
        for(int i=min(z->resultLabel->X0,z->resultLabel->X1); i<max(z->resultLabel->X0,z->resultLabel->X1); i++) {
            for(int j=min(z->resultLabel->Y0,z->resultLabel->Y1); j<max(z->resultLabel->Y0,z->resultLabel->Y1); j++)
                resImage.setPixel(QPoint(x,y++),z->image.pixel(i,j));
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
