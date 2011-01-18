#include "controleur.h"
#include "zonedessin.h"
#include "matconvo.h"
#include "noyaupascal.h"
#include <QtGui>

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
    sX0 = -1;
    sX1 = -1;
    sY0 = -1;
    sY1 = -1;
    //histogramme = new Histogramme (z->image);
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

void Controleur::appliquer_median()
{
    qDebug()<<"fonction appliquer_median;";
    QImage imFiltree = z->image.copy(0,0,z->image.width(),z->image.height());
    int gris;
    int i,j, k, l;
    // ATTENTION verifier que l'image est IsGreyScale() avant d'appeler
    // cette fonction.


    // rentrer taille du filtre median
    int taille = 3;
    int nv = taille*taille;
    int distPixel = (int) taille/2;
    qDebug()<<"taille filtre: "<<taille<<" ; NbVoisins: "<<nv<<" ; distPixel : "<<distPixel;

    MatConvo *voisins = new MatConvo();
    voisins->allouerMem(nv,1);
    for (i=0 ; i<=imFiltree.width()-1 ; i++)
    {
        for (j=0 ; j<=imFiltree.height()-1 ; j++)
        {
            voisins->setTCourante(0);
            for (k=-distPixel ; k<=distPixel ; k++)
            {
                for(l=-distPixel ; l<=distPixel ; l++)
                {
                    if ((i+k>=0) && (i+k<=z->image.width()-1) &&
                        (j+l>=0) && (j+l<=z->image.height()-1))
                    {
                        voisins->ajouter_gris(qRed(z->image.pixel(i+k,j+l)));
                    }
                }
            }
            if (voisins->getTCourante() != 0)
            {
                //voisins->ranger_gris();
                gris = voisins->gris_median((int) voisins->getTCourante()/2);
                imFiltree.setPixel(i,j,qRgb((int)gris,(int)gris,(int)gris));
            }
        }
    }
    z->image = imFiltree;
    z->afficher_image();
    voisins->~MatConvo();
}

void Controleur::appliquer_flou()
{
    QImage imFloue = z->image.copy(0,0,z->image.width(),z->image.height());
    int i,j,k,l, s, t;
    int compteur;
    double r,g,b; // composantes de la nouvelle couleur
    TypeConvo tConv = GAUSS;
    MatConvo *m = new MatConvo();

    if(tConv == MOYENNE){
        // t est la taille de la matrice qui devra etre rentré par l'utilisateur...
        int tf = 3;
        m->allouerMem(tf,1);
        m->noyau_moyenne();
    }
    else if( tConv == GAUSS){
        // coef est le coef max du noyau de gauss entré par l'utilisateur...
        int coef = -12;
        NoyauPascal p(coef);
        p.calcul_taille();
        m->allouerMem(p.getTaille(),p.getCoef());
        m->noyau_coef();
        m->noyau_gauss_bruit();
    }
    else
        qDebug()<<"erreur, aucune matrice n'a ete initialisee";

    int distPixel = (int) m->getTFiltre()/2;
    qDebug()<< "taille du filtre:"<< m->getTFiltre()<< " ; distance du pixel central:"<< distPixel;

    // On applique le filtre choisi taille t (à 1) sur l'image
    for(i=0 ; i<=imFloue.width()-1 ; i++)
    {
        for(j=0 ; j<=imFloue.height()-1 ; j++)
        {
            // On fait la somme des taille^2 pixels (des taille^2 qui
            // entourent le point sur lequel on est entrain d'appliquer
            // le filtre et lui même, seulement s'ils ne sont pas en
            // dehors de la zone image)
            r = g = b = 0;
            compteur = 0;
            for (int x=0 ; x<m->getTFiltre() ; x++)
                for (int y=0 ; y<m->getTFiltre() ; y++)
                    compteur += m->getMat2(x,y);

            //qDebug()<<"compteur: "<<compteur;

            for (k=-distPixel, s=0 ; k<=distPixel && s<m->getTFiltre(); k++, s++)
            {
                for(l=-distPixel, t=0 ; l<=distPixel && t<m->getTFiltre() ; l++, t++)
                {
                    if ((i+k>=0) && (i+k<=z->image.width()-1) &&
                        (j+l>=0) && (j+l<=z->image.height()-1))
                    {
                        r = r + (m->getMat2(s,t)*qRed(z->image.pixel(i+k,j+l)));
                        g = g + (m->getMat2(s,t)*qGreen(z->image.pixel(i+k,j+l)));
                        b = b + (m->getMat2(s,t)*qBlue(z->image.pixel(i+k,j+l)));
                    }
                    else compteur = compteur - m->getMat2(s,t);
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
    z->image = imFloue;
    z->afficher_image();
    m->~MatConvo();
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


QImage Controleur::redimensionner(int l, int h)
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

    return res;
}
