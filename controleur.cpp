#include "controleur.h"
#include "zonedessin.h"
#include "matconvo.h"
#include "noyaupascal.h"

Controleur::Controleur(ZoneDessin *zone)
{
    z = zone;
    f = new Filtres();
    red = new Redim(z->image);
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
    case DECOUPAGE_INTEL: {
            decoupage_intelligent_clic(z->resultLabel->X1,z->resultLabel->Y1);
            this->changer_mode(SELECTION);
            break;
        }
    case BAGUETTE_MAGIQUE: {
            decoupage_intelligent(z->resultLabel->X1,z->resultLabel->Y1);
            this->changer_mode(SELECTION);
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

MatConvo *Controleur::creer_laplacien(int numero, int alpha)
{
    return this->f->creer_laplacien(numero, alpha);
}

MatConvo *Controleur::creer_rehausseur_laplacien(int numero, int alpha)
{
    return this->f->creer_rehausseur_laplacien(numero, alpha);
}

MatConvo *Controleur::creer_impulsionnel()
{
    return this->f->creer_impulsionnel();
}

MatConvo *Controleur::creer_gradient_x(TypeConvo tConv)
{
    return this->f->creer_gradient_x(tConv);
}

MatConvo *Controleur::creer_gradient_y(TypeConvo tConv)
{
    return this->f->creer_gradient_y(tConv);
}
void Controleur::appliquer_median(int taille)
{
    if (z->image.isGrayscale()){
        z->changer_image(f->appliquer_median(taille, z->image));
    }
    else
        emit changer_message_barre("L'image doit �tre en niveaux de gris!",0);
}

void Controleur::appliquer_flou(int taille,TypeConvo tConv)
{
    z->changer_image(f->appliquer_flou(creer_filtre(taille,tConv),z->image));
}

void Controleur::appliquer_flou(MatConvo *m)
{
    z->changer_image(f->appliquer_flou(m,z->image));
}

void Controleur::appliquer_filtre(MatConvo *m)
{
    z->changer_image(f->appliquer_filtre(m,z->image));
}

void Controleur::hysteresis(int seuilBas, int seuilHaut, int i)
{
    QImage imNorme = this->f->norme_4gradients(GRADIENT_PREWITT,z->image);
    QImage *contours = this->f->hysteresis(seuilBas,seuilHaut,imNorme);
    z->changer_image(contours[i]);
}

void Controleur::chainage_contours(int seuilBas, int seuilHaut, TypeConvo tConv)
{
    //z->changer_image(this->f->appliquer_median(3,z->image));
    z->changer_image(this->f->chainage_contours(seuilBas,seuilHaut,tConv,z->image));
}

void Controleur::appliquer_rehaussement(int alpha)
{
    z->changer_image(this->f->appliquer_rehaussement(alpha,z->image));
}

void Controleur::appliquer_laplacien(MatConvo *m)
{
    z->changer_image(this->f->appliquer_laplacien(m,z->image));
}

void Controleur::appliquer_gradient_x(TypeConvo tConv)
{
    z->changer_image(this->f->RGB_to_grey(f->appliquer_filtre(this->creer_gradient_x(tConv), z->image)));
}

void Controleur::appliquer_gradient_y(TypeConvo tConv)
{
    z->changer_image(this->f->RGB_to_grey(f->appliquer_filtre(this->creer_gradient_y(tConv), z->image)));
}

void Controleur::appliquer_gradient_moins_x(TypeConvo tConv)
{
    z->changer_image(this->f->RGB_to_grey(f->appliquer_filtre(this->f->creer_gradient_moins_x(tConv), z->image)));
}

void Controleur::appliquer_gradient_moins_y(TypeConvo tConv)
{
    z->changer_image(this->f->RGB_to_grey(f->appliquer_filtre(this->f->creer_gradient_moins_y(tConv), z->image)));
}

void Controleur::norme_gradient(TypeConvo tConv)
{
    QImage imX = this->f->appliquer_filtre(this->creer_gradient_x(tConv),z->image);
    QImage imY = this->f->appliquer_filtre(this->creer_gradient_y(tConv),z->image);
    z->changer_image(this->f->norme_gradient(imX,imY));
}

void Controleur::norme_4gradients(TypeConvo tConv)
{
    QImage imX = this->f->appliquer_filtre(this->creer_gradient_x(tConv),z->image);
    QImage imY = this->f->appliquer_filtre(this->creer_gradient_y(tConv),z->image);
    QImage imNorme1 = this->f->norme_gradient(imX,imY);
    QImage imMoinsX = this->f->appliquer_filtre(this->f->creer_gradient_moins_x(tConv),z->image);
    QImage imMoinsY = this->f->appliquer_filtre(this->f->creer_gradient_moins_y(tConv),z->image);
    QImage imNorme2 = this->f->norme_gradient(imMoinsX,imMoinsY);
    z->changer_image(this->f->norme_gradient(imNorme1,imNorme2));
}

void Controleur::supp_non_maxima(TypeConvo tConv)
{
    QImage imX = this->f->appliquer_filtre(this->creer_gradient_x(tConv),z->image);
    QImage imY = this->f->appliquer_filtre(this->creer_gradient_y(tConv),z->image);
    QImage imNorme = this->f->norme_gradient(imX,imY);
    z->changer_image(this->f->supp_non_maxima(imX,imY,imNorme));
}
void Controleur::passage_zero(int numero,int alpha, int seuil)
{
    QImage laplacien = this->f->appliquer_laplacien(this->creer_laplacien(numero,alpha),z->image);

    z->changer_image(this->f->passage_zero(seuil, laplacien));
}

void Controleur::eclaircir(int alpha)
{
    z->changer_image(this->f->eclaircir(alpha,z->image));
}

void Controleur::mon_seuillage(TypeConvo tConv, int seuil)
{
    QImage imNorme = this->f->norme_4gradients(tConv,z->image);
    z->changer_image(this->f->seuillage(imNorme,seuil));
}
void Controleur::seuillage(int s){
    QImage res = z->image.copy();
    double greyD;
    int i,j;

    for (i=0 ; i<res.width() ; i++)
    {
        for(j=0 ; j<res.height() ; j++)
        {
            QColor c = res.pixel(i,j);
            //transformer en niveaux de gris
            greyD = c.red()*0.299 + c.green()*0.587 + c.blue()*0.114 + 0.5;
            //tous les points au desssus d'un certain niveau blancs, les autres noirs
            if (greyD>s)
                res.setPixel(i,j,qRgb(255,255,255));
            else
                res.setPixel(i,j,qRgb(0,0,0));
        }
    }
    //changer_image
    z->changer_image(res);
}

void Controleur::luminosite_contraste(float lum, float cont){
    //parametres:
    //lum: de 0.0 (image noire) a 1.0 (image blanche)
    //cont: de 0.0 (pas de contraste) a 1.0 (tres contrastee)
    QImage res(z->image.width(),z->image.height(),z->image.format());
    int i,j;
    int r, g, b, moy_r=0, moy_g=0, moy_b=0;

    //mettre lum et cont sous une autre forme pour la formule, i.e.
    //luminosite: -1.0: l'image noire, 1.0: l'image blanche, 0.0: normal
    //contraste: 0.0: pas de contraste (gris), 1.0: l'image normal, 2.0: tres contrastee
    lum=lum*2-1.0;
    cont=cont*2;

    if(!z->image.isNull()){

        //calculer les moyennes R,G,B dans l'image
        for (i=0 ; i<z->image.width() ; i++)
        {
            for(j=0 ; j<z->image.height() ; j++)
            {
                QColor c = z->image.pixel(i,j);
                moy_r+=c.red();
                moy_g+=c.green();
                moy_b+=c.blue();
            }
        }
        i=z->image.width()*z->image.height();//optimisation
        moy_r=moy_r/i;
        moy_g=moy_g/i;
        moy_b=moy_b/i;

        //modifier chaque point de l'image
        for (i=0 ; i<z->image.width() ; i++)
        {
            for(j=0 ; j<z->image.height() ; j++)
            {
                QColor c = z->image.pixel(i,j);
                r=c.red();
                g=c.green();
                b=c.blue();

                //modifier contraste par rapport a la moyenne, rajouter ou supprimer de la lum
                r=((r-moy_r)*cont+moy_r)+255*lum;
                g=((g-moy_g)*cont+moy_g)+255*lum;
                b=((b-moy_b)*cont+moy_b)+255*lum;

                //ajustement des valeurs resultats qui debordent
                if (r>255)
                    r=255;
                else{
                    if (r<0)
                        r=0;
                }
                if (g>255)
                    g=255;
                else{
                    if (g<0)
                        g=0;
                }
                if (b>255)
                    b=255;
                else{
                    if (b<0)
                        b=0;
                }

                res.setPixel(i,j,qRgb(r,g,b));
            }
        }
    }
    z->changer_image(res);
}

void Controleur::decoupage_intelligent_contours()
{
    //z->changer_image(this->decouper());

    QImage imIn = this->decouper();
    z->changer_image(this->f->decoupage_intelligent_contours(imIn));
}
void Controleur::decoupage_intelligent_clic(int x, int y)
{
    //z->changer_image(this->decouper());

    z->changer_image(this->f->decoupage_intelligent_clic(z->image, x, y));
}

void Controleur::decoupage_intelligent(int x, int y)
{
    z->changer_image(this->f->decoupage_intelligent(z->image, x, y));
}

QImage Controleur::decouper()
{
    red->maj(z->image);
    QImage resImage;

    if(z->resultLabel->X0 == z->resultLabel->X1 || z->resultLabel->Y0 == z->resultLabel->Y1)
        resImage = z->image;
    else {
        resImage = red->decouper(sX0,sY0,sX1,sY1);
    }

    return resImage;
}


void Controleur::redimensionner(int l, int h, int mode)
{
    QImage res(l,h,z->image.format());

    red->maj(z->image);
    if (mode == 0) {


        if (l<z->image.width() && h<z->image.height()) // si la diminution est sur les deux taille
            res = red->down(l ,h);
        else if (l < z->image.width()) // si elle n'est que sur la largeur
            res = red->down(l ,z->image.height());
        else if (h < z->image.height()) // si elle n'est que sur la hauteur
            res = red->down(z->image.width(),h);
        else
            res = z->image;
        if (l<z->image.width() && h<z->image.height()) // si la diminution est sur les deux taille
            res = red->down(l ,h);
        else if (l < z->image.width()) // si elle n'est que sur la largeur
            res = red->down(l ,z->image.height());
        else if (h < z->image.height())  // si elle n'est que sur la hauteur
            res = red->down(z->image.width(),h);
        //red->maj(z->image);

        if (l>z->image.width() && h>z->image.height()) {// meme sh�ma
            red->up(l, h);
        }
        else if (l > z->image.width())
            res = red->up(l, z->image.height());
        else if (h > z->image.height())
            res = red->up(z->image.height(), h);
        if (l>z->image.width() && h>z->image.height()) // meme sh�ma
            res = red->up(l,h);
        else if (l > z->image.width())
            res = red->up(l, z->image.height());
        else if (h > z->image.height())
            res = red->up(z->image.height(), h);
    }
    else if (mode == 1){
        res = red->interpolationBilineaire(l,h);
    }
    else if (mode == 2){
        res = red->redimSeamCarving(z->image.width()-l,z->image.height()-h);
    }
    z->changer_image(res);
}

void Controleur::appliquer_rehausseur_laplacien(int num, int alpha){
    appliquer_filtre(creer_rehausseur_laplacien(num, alpha));
}

void Controleur::appliquer_laplacien(int num, int intensite){
    appliquer_filtre(creer_laplacien(num,intensite));
}
