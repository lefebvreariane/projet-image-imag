#include "controleur.h"
#include "zonedessin.h"
#include "matconvo.h"
#include "noyaupascal.h"

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
    sX0 = -1;
    sX1 = -1;
    sY0 = -1;
    sY1 = -1;
}

void Controleur::reInitSelection()
{
    sX0 = -1;
    sX1 = -1;
    sY0 = -1;
    sY1 = -1;
}

void Controleur::changer_mode(Mode m )
{
    mode = m;
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
            z->init_affichage();
            QPainter paint(&(z->image_affichages));
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
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::black);
            paint.setPen(pen);


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
    emit afficher_pixel(pixel.red(), pixel.green(),pixel.blue());

}

void Controleur::RGB_to_grey()
{
    z->changer_image(f->RGB_to_grey(z->image));
}

void Controleur::inverser_couleurs()
{
    QImage imIn = z->image.copy(0,0,z->image.width(),z->image.height());
    QImage imOut = z->image.copy(0,0,z->image.width(),z->image.height());

    for (int i=0 ; i<z->image.width() ; i++)
    {
        for(int j=0 ; j<z->image.height() ; j++)
        {
            imOut.setPixel(i,j,qRgb(255-qRed(imIn.pixel(i,j)),255-qGreen(imIn.pixel(i,j)),255-qBlue(imIn.pixel(i,j))));
        }
    }
    z->changer_image(imOut);
}

MatConvo *Controleur::creer_filtre(int coefOuTaille, TypeConvo tConv)
{
    return this->f->creer_filtre(coefOuTaille,tConv);
}

MatConvo *Controleur::creer_laplacien(int numero)
{
    return this->f->creer_laplacien(numero);
}

MatConvo *Controleur::creer_impulsionnel()
{
    return this->f->creer_impulsionnel();
}
void Controleur::appliquer_median(int taille)
{
    if (z->image.isGrayscale()){
        qDebug()<<"fonction appliquer_median;";
        z->changer_image(f->appliquer_median(taille, z->image));
    }
    else
        emit changer_message_barre("L'image doit être en niveaux de gris!",0);
}

void Controleur::appliquer_flou(int taille,TypeConvo tConv)
{
    qDebug()<<"fonction appliquer_flou;";
    z->changer_image(f->appliquer_flou(creer_filtre(taille,tConv),z->image));
}

void Controleur::appliquer_flou(MatConvo *m)
{
    qDebug()<<"fonction appliquer_flou;";
    z->changer_image(f->appliquer_flou(m,z->image));
}



void Controleur::seuillage(int seuil)
{
    qDebug()<<"fonction rehaussement_contraste;";
    z->changer_image(f->seuillage(seuil, z->image));
}

void Controleur::rehaussement_contraste()
{
    qDebug()<<"fonction rehaussement_contraste;";
    z->changer_image(f->rehaussement_contraste(z->image));
}

/*void Controleur::appliquer_laplacien(int numero)
{
    qDebug()<<"fonction appliquer_laplacien;";
    z->image = this->f->appliquer_laplacien(numero,z->image);
    z->afficher_image();
}*/

void Controleur::appliquer_rehaussement(int alpha)
{
    qDebug()<<"fonction appliquer_laplacien;";
    z->changer_image(f->appliquer_rehaussement(alpha,z->image));
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

QImage down(QImage base, int l, int h) {

    QImage res(l,h,base.format());
    double ratioL, ratioH;
    double compteurH = 0, compteurL = 0;
    int iR = 0, jR = 0;

    //Reduction de la taille de l'image :
    ratioL = (double) base.width() / (double) l;
    ratioH = (double) base.height() / (double) h ;

    int cptL = 0, cptH = 0;


    for(int i = 0; i < l; i++) {
        for(int j = 0; j < h; j++) {
            int r = 0, g = 0, b = 0;
            compteurL += ratioL;
            compteurH += ratioH;
            //qDebug() << compteurL << "," << ratioL;
            //if (iR > 330)
            //  return res;

            //qDebug() << compteurL <<"," << compteurH;
            for(cptL = 0; cptL < (int) compteurL/* && cptL+iR < base.width()*/; cptL++) {
                for(cptH = 0; cptH < (int) compteurH/* && cptH+jR < base.height()*/; cptH++) {
                    r += ((QColor) base.pixel(cptL+iR, cptH+jR)).red();
                    g += ((QColor) base.pixel(cptL+iR, cptH+jR)).green();
                    b += ((QColor) base.pixel(cptL+iR, cptH+jR)).blue();
                    //        qDebug() << "lecture en " << cptL+iR << "," << cptH+jR << '!' << compteurL << "," << compteurH;
                }
            }

            jR = (int) ((double)  j*(ratioH)) ;
            //qDebug() << "ecriture en " << i << "," << j;
            res.setPixel(QPoint(i,j), (QColor(r/(cptL*cptH),g/(cptL*cptH),b/(cptL*cptH),255).rgb()));
            //qDebug() << compteurL << (int) compteurL;
            compteurL -= (int) compteurL;

            compteurH -= (int) compteurH;
        }
        //qDebug() << iR << "," << cptL;
        iR = (int) ((double) i*(ratioL ));

        jR = 0;


    }

    return res;
}

QImage up(QImage base, int l, int h) {

    QImage res(l,h,base.format());
    double ratioL, ratioH;
    double compteurH = 0, compteurL = 0;
    int iR = 0, jR = 0;

    //Augmentation de la taille de l'image :
    ratioH = (double) base.height() / ((double) h - base.height());
    ratioL = (double) base.width() / ((double) l - base.width());
    for(int iB=0; iB < base.width(); iB++) {
        for(int jB=0; jB<base.height(); jB++) {
            // On parcours l'image et copie chaque pixel
            res.setPixel(QPoint(iR,jR++),base.pixel(iB,jB));
            //S'il y a un redimensionnement sur la hauteur on rajoute les pixels necessaire
            if (h > base.height()) {
                compteurH ++;
                while (compteurH >= ratioH) {
                    res.setPixel(QPoint(iR,jR++),base.pixel(iB,jB));
                    compteurH -= ratioH;
                }
            }
        }
        //Dans le cas d'un nombre de pixel impair la hauteur de l'image ne sera pas atteinte
        if (jR != res.height()) {
            res.setPixel(QPoint(iR,jR),res.pixel(iR,jR-1));
            jR++;
        }
        //S'il y a un redimensionnement sur la largeur on rajoute les pixels necessaire
        if (l > base.width()) {
            compteurL ++;
            while (compteurL >= ratioL) {
                iR++;
                for(int j=0 ; j < res.height(); j++)
                    res.setPixel(QPoint(iR,j),res.pixel(iR-1,j));
                compteurL -= ratioL;
            }
        }
        iR++;
        jR = 0;
        compteurH = 0;
    }
    //Dans le cas d'un nombre de pixel impaire la largeur de l'image ne sera pas atteinte
    if (iR != res.width()) {
        for(int j=0 ; j < res.height(); j++)
            res.setPixel(QPoint(iR,j),res.pixel(iR-1,j));
    }

    return res;
}



void Controleur::redimensionner(int l, int h)
{
    QImage res(l,h,z->image.format());



    if (l<z->image.width() && h<z->image.height()) // si la diminution est sur les deux taille
        res = down(z->image,l ,h);
    else if (l < z->image.width()) // si elle n'est que sur la largeur
        res = down(z->image,l ,z->image.height());
    else if (h < z->image.height()) // si elle n'est que sur la hauteur
        res = down(z->image,z->image.width(),h);
    else
        res = z->image;

    if (l>z->image.width() && h>z->image.height()) // meme shéma
        res = up(res,l, h);
    else if (l > z->image.width())
        res = up(res, l, z->image.height());
    else if (h > z->image.height())
        res = up(res, z->image.height(), h);

    z->changer_image(res);
}
