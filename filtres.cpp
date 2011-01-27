#include "filtres.h"
#include "matconvo.h"
#include "noyaupascal.h"
#include <QtGui>

Filtres::Filtres()
{
}

QImage Filtres::RGB_to_grey(QImage imIn)
{   //lit l'image qui correspond au LabelImage (z->resultLabel)
    //traite l'image
    //affiche l'image correspondante

    if(!imIn.isNull() && !imIn.allGray())
    {
        double greyD;
        int greyI,i,j;

        for (i=0 ; i<imIn.width() ; i++)
        {
            for(j=0 ; j<imIn.height() ; j++)
            {
                QColor c = imIn.pixel(i,j);
                greyD = c.red()*0.299 + c.green()*0.587 + c.blue()*0.114 + 0.5;
                greyI = (int) greyD;
                imIn.setPixel(i,j,qRgb(greyI,greyI,greyI));
            }
        }
    }
    return imIn;
}

MatConvo *Filtres::creer_filtre(int coefOuTaille, TypeConvo tConv)
{
    MatConvo *m = new MatConvo();
    if(tConv == MOYENNE){
        m->allouerMem(coefOuTaille,1);
        m->noyau_moyenne();
        return m;
    }
    else if( tConv == GAUSS){
        NoyauPascal p(coefOuTaille);
        p.calcul_taille();
        m->allouerMem(p.getTaille(),p.getCoef());
        m->noyau_coef();
        m->noyau_gauss_bruit();
        return m;
    }
    else {
        qDebug()<<"erreur, aucune matrice n'a pas ete initialisee";
        return NULL;
    }
}

MatConvo *Filtres::creer_laplacien(int numero, int alpha)
{
    MatConvo *m = new MatConvo();
    if(numero == 1 || numero == 2 || numero == 3){
        m->allouerMem(3,alpha);
        m->noyau_laplacien(numero);
        return m;
    }
    else {
        qDebug()<<"erreur, aucune matrice n'a pas ete initialisee";
        return NULL;
    }
}

MatConvo *Filtres::creer_rehausseur_laplacien(int numero, int alpha)
{
    MatConvo *m = this->creer_laplacien(numero, alpha);
    for (int i=0 ; i<m->getTFiltre() ; i++)
        for (int j=0 ; j<m->getTFiltre() ; j++)
            m->setMat2(i,j,m->getMat2(i,j)*-1);
    m->setMat2(1,1,m->getMat2(1,1)+alpha);
    return m;
}

MatConvo *Filtres::creer_impulsionnel()
{
    MatConvo *m = new MatConvo();
    m->allouerMem(3,1);
    m->noyau_impulsionnel();
    return m;
}

QImage Filtres::appliquer_median(int taille, QImage imIn)
{
    int nbConvo = 1;
    int n=0;
    if (taille == 15)
    {
        taille = 7;
        nbConvo = 2;
    }
    else if (taille == 13)
    {
        taille = 5;
        nbConvo = 3;
    }
    else if (taille == 11)
    {
        taille = 5;
        nbConvo = 2;
    }
    QImage imFiltree = imIn.copy(0,0,imIn.width(),imIn.height());
    int gris;
    int i,j, k, l;
    int nv = taille*taille;
    int distPixel = (int) taille/2;

    MatConvo *voisins = new MatConvo();
    voisins->allouerMem(nv,1);

    while (n<nbConvo)
    {
        imIn = imFiltree.copy(0,0,imIn.width(),imIn.height());


        for (i=0 ; i<=imFiltree.width()-1 ; i++)
        {
            for (j=0 ; j<=imFiltree.height()-1 ; j++)
            {
                voisins->setTCourante(0);
                for (k=-distPixel ; k<=distPixel ; k++)
                {
                    for(l=-distPixel ; l<=distPixel ; l++)
                    {
                        if ((i+k>=0) && (i+k<imIn.width()) &&
                            (j+l>=0) && (j+l<imIn.height()))
                        {
                            voisins->ajouter_val_mat1(qRed(imIn.pixel(i+k,j+l)));
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
        n++;
    }
    voisins->~MatConvo();
    return imFiltree;
}

QImage Filtres::appliquer_flou(MatConvo *m, QImage imIn)
{
    QImage imFloue = imIn.copy(0,0,imIn.width(),imIn.height());
    int i,j,k,l, s, t;
    int compteur;
    double r,g,b; // composantes de la nouvelle couleur

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
                    if ((i+k>=0) && (i+k<imIn.width()) &&
                        (j+l>=0) && (j+l<imIn.height()))
                    {
                        r = r + (m->getMat2(s,t)*qRed(imIn.pixel(i+k,j+l)));
                        g = g + (m->getMat2(s,t)*qGreen(imIn.pixel(i+k,j+l)));
                        b = b + (m->getMat2(s,t)*qBlue(imIn.pixel(i+k,j+l)));
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
    m->~MatConvo();
    return imFloue;
}

QImage Filtres::appliquer_laplacien(MatConvo *m, QImage imIn)
{
    QImage imFiltree = imIn.copy(0,0,imIn.width(),imIn.height());
    int i,j,k,l,s,t;
    double r,g,b;

    int distPixel = (int) m->getTFiltre()/2;

    for (i=distPixel ; i<imFiltree.width()-distPixel ; i++)
    {
        for (j=distPixel ; j<imFiltree.height()-distPixel ; j++)
        {
            r=0; g=0;b=0;
            for (k=-distPixel, s=0 ; k<=distPixel && s<m->getTFiltre(); k++, s++)
            {
                for(l=-distPixel, t=0 ; l<=distPixel && t<m->getTFiltre() ; l++, t++)
                {
                    r = r + (m->getMat2(s,t)*qRed(imIn.pixel(i+k,j+l)));
                    g = g + (m->getMat2(s,t)*qGreen(imIn.pixel(i+k,j+l)));
                    b = b + (m->getMat2(s,t)*qBlue(imIn.pixel(i+k,j+l)));
                }
            }
            r = r/m->getCoef()+125;
            if (r < 0)
                r = 0;
            else if (r>255)
                r = 255;
            g = g/m->getCoef()+125;
            if (g < 0)
                g = 0;
            else if (g>255)
                g = 255;
            b = b/m->getCoef()+125;
            if (b < 0)
                b = 0;
            else if (b>255)
                b = 255;
            imFiltree.setPixel(i,j,qRgb((int)r,(int)g,(int)b));
        }
    }
    imFiltree = this->RGB_to_grey(imFiltree);
    m->~MatConvo();
    return imFiltree;
}

QImage Filtres::passage_zero(int seuil, QImage laplacien)
{
    QImage imOut = laplacien.copy(0,0,laplacien.width(),laplacien.height());

    for (int i=1 ; i<laplacien.width()-1 ; i++)
    {
        for (int j=1 ; j<laplacien.height()-1 ; j++)
        {
            if (abs(laplacien.pixel(i,j-1)-laplacien.pixel(i,j)) > seuil ||       /*en x*/
                abs(laplacien.pixel(i-1,j)-laplacien.pixel(i,j)) > seuil ||       /*en y*/
                abs(laplacien.pixel(i-1,j-1)-laplacien.pixel(i,j)) > seuil ||     /*en diag de H-G a B-D */
                abs(laplacien.pixel(i+1,j+1)-laplacien.pixel(i,j)) > seuil ||     /*en diag de H-D a B-G */
                abs(laplacien.pixel(i,j+1)-laplacien.pixel(i,j)) > seuil ||       /*en -x*/
                abs(laplacien.pixel(i+1,j)-laplacien.pixel(i,j)) > seuil ||       /*en -y*/
                abs(laplacien.pixel(i+1,j-1)-laplacien.pixel(i,j)) > seuil ||     /*en -diag de H-G a B-D */
                abs(laplacien.pixel(i-1,j+1)-laplacien.pixel(i,j)) > seuil)       /*en -diag de H-D a B-G */
            {
                imOut.setPixel(i,j,qRgb(255,255,255));
            }
            else
                imOut.setPixel(i,j,qRgb(0,0,0));
        }
    }
    return imOut;
}

/*QImage Filtres::info_contours(QImage imIn)
{
    //this->RGB_to_grey();
    int r,g,b;
    QImage imOut = imIn.copy(0,0,imIn.width(),imIn.height());
    imIn = this->appliquer_flou(this->creer_filtre(2,GAUSS),imIn);
    for (int i=0 ; i<imIn.width() ; i++)
    {
        for(int j=0 ; j<imIn.height() ; j++)
        {
            r = 2*(qRed(imOut.pixel(i,j)) - qRed(imIn.pixel(i,j)));
            g = 2*(qGreen(imOut.pixel(i,j)) - qGreen(imIn.pixel(i,j)));
            b = 2*(qBlue(imOut.pixel(i,j)) - qBlue(imIn.pixel(i,j)));
            imOut.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return imOut;
}*/

QImage *Filtres::hysteresis(int seuilBas, int seuilHaut, QImage imNorme)
{
    QImage *Out = new QImage[2];

    Out[0] = imNorme.copy(0,0,imNorme.width(),imNorme.height()); // estPotentiel
    Out[1] = imNorme.copy(0,0,imNorme.width(),imNorme.height()); // estContour

    for (int i=0 ; i<imNorme.width() ; i++)
    {
        for (int j=0 ; j<imNorme.height() ; j++)
        {
            if (qRed(imNorme.pixel(i,j)) > seuilHaut)
            {
                Out[1].setPixel(i,j,qRgb(255,255,255));
                Out[0].setPixel(i,j,qRgb(0,0,0));
            }
            else if (qRed(imNorme.pixel(i,j)) >= seuilBas && qRed(imNorme.pixel(i,j)) <= seuilHaut)
            {
                Out[1].setPixel(i,j,qRgb(0,0,0));
                Out[0].setPixel(i,j,qRgb(255,255,255));
            }
            else
            {
                Out[1].setPixel(i,j,qRgb(0,0,0));
                Out[0].setPixel(i,j,qRgb(0,0,0));
            }
        }
    }
    //imOut = this->appliquer_median(3,imOut);
    return Out;
}

QImage Filtres::chainage_contours(int seuilBas, int seuilHaut, TypeConvo tConv, QImage imIn)
{
    QImage imNorme;
    if (tConv == LAPLACIEN)
    {
        imNorme = this->appliquer_filtre(this->creer_laplacien(2,1), imIn);
    }
    else
        imNorme = this->norme_4gradients(tConv,imIn);

    QImage *contours = this->hysteresis(seuilBas, seuilHaut, imNorme);
    //contours[0] -> estPotentiel // contours[1] -> estContour

    //1er passage
    for(int i=1 ; i<imIn.width()-1 ; i++)
        for(int j=1 ; j<imIn.height() ; j++)
            if(qRed(contours[0].pixel(i,j) == 1))
                if((contours[0].pixel(i,j-1) == 1) || (contours[0].pixel(i-1,j-1) == 1) ||
                   (contours[0].pixel(i-1,j) == 1) || (contours[0].pixel(i-1,j+1) == 1))
                     contours[1].setPixel(i,j,qRgb(255,255,255));

    //2eme passage
    for(int j=imIn.height()-2 ; j>=0 ; j--)
        for(int i=1 ; i<imIn.width()-1 ; i++)
            if(qRed(contours[0].pixel(i,j) == 1))
                if((contours[0].pixel(i-1,j) == 1) || (contours[0].pixel(i-1,j+1) == 1) ||
                   (contours[0].pixel(i,j+1) == 1) || (contours[0].pixel(i+1,j+1) == 1))
                     contours[1].setPixel(i,j,qRgb(255,255,255));

    //3eme passage
    for(int i=imIn.width()-2 ; i>=0 ; i--)
        for(int j=imIn.height()-2 ; j>0 ; j--)
            if(qRed(contours[0].pixel(i,j) == 1))
                if((contours[0].pixel(i,j+1) == 1) || (contours[0].pixel(i+1,j+1) == 1) ||
                   (contours[0].pixel(i+1,j) == 1) || (contours[0].pixel(i+1,j-1) == 1))
                     contours[1].setPixel(i,j,qRgb(255,255,255));

    //4eme passage
    for(int j=1 ; j<imIn.height() ; j++)
        for(int i=imIn.width()-2 ; i>0 ; i--)
            if(qRed(contours[0].pixel(i,j) == 1))
                if((contours[0].pixel(i+1,j) == 1) || (contours[0].pixel(i+1,j-1) == 1) ||
                   (contours[0].pixel(i,j-1) == 1) || (contours[0].pixel(i-1,j-1) == 1))
                     contours[1].setPixel(i,j,qRgb(255,255,255));

    return contours[1];

}

QImage Filtres::appliquer_rehaussement(int alpha, QImage imIn)
{
    MatConvo *m = this->creer_filtre(3,MOYENNE);
    MatConvo *d = this->creer_impulsionnel();

    d->setMat2(1,1,alpha+1);
    for (int i=0 ; i<m->getTFiltre() ; i++)
        for(int j=0 ; j<m->getTFiltre() ; j++)
            m->setMat2(i,j,(m->getMat2(i,j)*d->getMat2(i,j))-1);

    QImage imOut = imIn.copy(0,0,imIn.width(),imIn.height());
    int i,j,k,l,s,t;
    double r,g,b; // composantes de la nouvelle couleur

    int distPixel = (int) m->getTFiltre()/2;

    for(i=distPixel ; i<imOut.width()-distPixel ; i++)
    {
        for(j=distPixel ; j<imOut.height()-distPixel ; j++)
        {
            r = g = b = 0;
            for (k=-distPixel, s=0 ; k<=distPixel && s<m->getTFiltre(); k++, s++)
            {
                for(l=-distPixel, t=0 ; l<=distPixel && t<m->getTFiltre() ; l++, t++)
                {
                    r = r + (m->getMat2(s,t)*qRed(imIn.pixel(i+k,j+l)));
                    g = g + (m->getMat2(s,t)*qGreen(imIn.pixel(i+k,j+l)));
                    b = b + (m->getMat2(s,t)*qBlue(imIn.pixel(i+k,j+l)));
                }
            }
            r = r/alpha ;//+125;
            if (r < 0)
                r = 0;
            if (r>255)
                r = 255;
            g = g/alpha ;//+ 125;
            if (g < 0)
                g = 0;
            if (g>255)
                g = 255;
            b = b/alpha ;//+ 125;
            if (b < 0)
                b = 0;
            if (b>255)
                b = 255;
            imOut.setPixel(i,j,qRgb((int)r,(int)g,(int)b));
        }
    }
    /*if (alpha <10)
        imOut = this->eclaircir(250,imOut);
    else if (alpha == 10)
        imOut = this->eclaircir(210, imOut);
    else if (alpha == 11)
        imOut = this->eclaircir(200, imOut);
    else if (alpha == 12)
        imOut = this->eclaircir(180, imOut);
    else if (alpha == 13)
        imOut = this->eclaircir(150, imOut);
    else if (alpha == 14)
        imOut = this->eclaircir(130, imOut);
    else if (alpha == 15)
        imOut = this->eclaircir(100, imOut);
    else if (alpha == 16)
        imOut = this->eclaircir(90, imOut);
    else if (alpha > 17)
        imOut = this->eclaircir(50, imOut);*/
    m->~MatConvo();
    d->~MatConvo();
    return imOut;
}

QImage Filtres::eclaircir(int alpha, QImage imIn)
{
    int r,g,b;
    QImage imOut = imIn.copy(0,0,imIn.width(),imIn.height());
    for (int i=0 ; i<imIn.width() ; i++)
    {
        for(int j=0 ; j<imIn.height() ; j++)
        {
            if (r>255) r = 255;
             else if(r<0) r = 0;
             else r = qRed(imIn.pixel(i,j)) + alpha;//*qRed(imIn.pixel(i,j))/100;
            if (r>255) r = 255;
            else if(r<0) r = 0;

            if (g>255) g = 255;
             else if(g<0) g = 0;
             else g = qGreen(imOut.pixel(i,j)) + alpha;//*qGreen(imIn.pixel(i,j))/100;
            if (g>255) g = 255;
            else if(g<0) g = 0;

            if (b>255) b = 255;
             else if(b<0) b = 0;
             else b = qBlue(imOut.pixel(i,j)) + alpha;//*qBlue(imIn.pixel(i,j))/100;
            if (b>255) b = 255;
            else if(b<0) b = 0;
            imOut.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return imOut;
}

QImage Filtres::appliquer_filtre(MatConvo *m, QImage imIn)
{
    QImage imOut = imIn.copy(0,0,imIn.width(),imIn.height());
    int i,j,k,l,s,t;
    double r,g,b; // composantes de la nouvelle couleur

    int distPixel = (int) m->getTFiltre()/2;

    for(i=distPixel ; i<imOut.width()-distPixel ; i++)
    {
        for(j=distPixel ; j<imOut.height()-distPixel ; j++)
        {
            r = g = b = 0;
            for (k=-distPixel, s=0 ; k<=distPixel && s<m->getTFiltre(); k++, s++)
            {
                for(l=-distPixel, t=0 ; l<=distPixel && t<m->getTFiltre() ; l++, t++)
                {
                    r = r + (m->getMat2(s,t)*qRed(imIn.pixel(i+k,j+l)));
                    g = g + (m->getMat2(s,t)*qGreen(imIn.pixel(i+k,j+l)));
                    b = b + (m->getMat2(s,t)*qBlue(imIn.pixel(i+k,j+l)));
                }
            }
            r = r/m->getCoef();
            if (r < 0)
                r = 0;
            if (r>255)
                r = 255;
            g = g/m->getCoef();
            if (g < 0)
                g = 0;
            if (g>255)
                g = 255;
            b = b/m->getCoef();
            if (b < 0)
                b = 0;
            if (b>255)
                b = 255;
            imOut.setPixel(i,j,qRgb((int)r,(int)g,(int)b));
        }
    }
    for (int k=0 ; k<imOut.width() ; k++)
        for (int l=0 ; l<distPixel ; l++)
        {
            imOut.setPixel(k,l,imOut.pixel(k,distPixel));
            imOut.setPixel(k,imOut.height()-l-1,imOut.pixel(k,imOut.height()-distPixel));
        }
    for (int k=0 ; k<distPixel ; k++)
        for (int l=0 ; l<imOut.height() ; l++)
        {
            imOut.setPixel(k,l,imOut.pixel(distPixel,l));
            imOut.setPixel(imOut.width()-k-1,l,imOut.pixel(imOut.width()-distPixel,l));
        }
    m->~MatConvo();
    return imOut;
}

MatConvo *Filtres::creer_gradient_x(TypeConvo tConv)
{
    MatConvo *m = new MatConvo();

    if (tConv == GRADIENT_SOBEL)
    {
        m->allouerMem(3,4);
        m->gradient_x_lisse(tConv);
        return m;
    }
    else if (tConv == GRADIENT_PREWITT)
    {
        m->allouerMem(3,3);
        m->gradient_x_lisse(tConv);
        return m;
    }
    else
        return NULL;
}
MatConvo *Filtres::creer_gradient_y(TypeConvo tConv)
{
    MatConvo *m = new MatConvo();

    if (tConv == GRADIENT_SOBEL)
    {
        m->allouerMem(3,4);
        m->gradient_y_lisse(tConv);
        return m;
    }
    else if (tConv == GRADIENT_PREWITT)
    {
        m->allouerMem(3,3);
        m->gradient_y_lisse(tConv);
        return m;
    }
    else
        return NULL;
}

MatConvo *Filtres::creer_gradient_moins_x(TypeConvo tConv)
{
    MatConvo *m = this->creer_gradient_x(tConv);
    for (int i=0 ; i<m->getTFiltre() ; i++)
        for (int j=0 ; j<m->getTFiltre() ; j++)
            m->setMat2(i,j,-m->getMat2(i,j));
    return m;
}

MatConvo *Filtres::creer_gradient_moins_y(TypeConvo tConv)
{
    MatConvo *m = this->creer_gradient_y(tConv);
    for (int i=0 ; i<m->getTFiltre() ; i++)
        for (int j=0 ; j<m->getTFiltre() ; j++)
            m->setMat2(i,j,-m->getMat2(i,j));
    return m;
}

QImage Filtres::norme_gradient(QImage imX, QImage imY)
{
    QImage imNorme = imX.copy(0,0,imX.width(),imX.height());

    int gris;

    for (int i=0 ; i<imNorme.width() ; i++)
    {
        for (int j=0 ; j<imNorme.height() ; j++)
        {
            gris = sqrt(qRed(imX.pixel(i,j))*qRed(imX.pixel(i,j))+qRed(imY.pixel(i,j))*qRed(imY.pixel(i,j)));
            imNorme.setPixel(i,j,qRgb(gris,gris,gris));
        }
    }
    return imNorme;
}
QImage Filtres::norme_4gradients(TypeConvo tConv, QImage imIn)
{
    //imIn = this->appliquer_median(3,imIn);
    QImage imX = this->appliquer_filtre(this->creer_gradient_x(tConv),imIn);
    imX = this->RGB_to_grey(imX);
    QImage imY = this->appliquer_filtre(this->creer_gradient_y(tConv),imIn);
    imY = this->RGB_to_grey(imY);
    QImage imNorme1 = this->norme_gradient(imX,imY);
    QImage imMoinsX = this->appliquer_filtre(this->creer_gradient_moins_x(tConv),imIn);
    imMoinsX = this->RGB_to_grey(imMoinsX);
    QImage imMoinsY = this->appliquer_filtre(this->creer_gradient_moins_y(tConv),imIn);
    imMoinsX = this->RGB_to_grey(imMoinsX);
    QImage imNorme2 = this->norme_gradient(imMoinsX,imMoinsY);
    return this->norme_gradient(imNorme1,imNorme2);
}

QImage Filtres::inverser_couleurs(QImage imIn)
{
    QImage imOut = imIn.copy(0,0,imIn.width(),imIn.height());

    for (int i=0 ; i<imIn.width() ; i++)
    {
        for(int j=0 ; j<imIn.height() ; j++)
        {
            imOut.setPixel(i,j,qRgb(255-qRed(imIn.pixel(i,j)),255-qGreen(imIn.pixel(i,j)),255-qBlue(imIn.pixel(i,j))));
        }
    }
    return imOut;
}

/*QImage Filtres::orientation(QImage gradH, QImage gradV)
{
    QImage direction(gradH.width(),gradH.height(),)
    for (int i=0, i<gradH.width() ; i++)
        atan(grad_x.pixel(x,y)/grad_y.pixel(x,y));
}*/

QImage Filtres::supp_non_maxima(QImage imX, QImage imY, QImage imNorme)
{
    int progress=0;
    QImage imOut = imX.copy(0,0,imX.width(),imX.height());

    for(int x = 1; x < imOut.width() - 1; x++) {
        progress++;
        for(int y = 1 ; y < imOut.height() - 1; y++) {
            int dx, dy;

            if(imX.pixel(x,y) > qRgb(0,0,0))
                dx = 1;
            else
                dx = -1;

            if(imY.pixel(x,y) > qRgb(0,0,0))
                dy = 1;
            else
                dy = -1;

            int a1, a2, b1, b2, A, B, point;
            QRgb val;
            if(abs(imX.pixel(x,y)) > abs(imY.pixel(x,y)))
            {
                a1 = imNorme.pixel(x+dx,y);
                a2 = imNorme.pixel(x+dx,y-dy);
                b1 = imNorme.pixel(x-dx,y);
                b2 = imNorme.pixel(x-dx,y+dy);
                A = (abs(imX.pixel(x,y)) - abs(imY.pixel(x,y)))*a1 + abs(imY.pixel(x,y))*a2;
                B = (abs(imX.pixel(x,y)) - abs(imY.pixel(x,y)))*b1 + abs(imY.pixel(x,y))*b2;
                point = imNorme.pixel(x,y) * abs(imX.pixel(x,y));
                if(point >= A && point > B) {
                    val = qRgb(qRed(abs(imX.pixel(x,y))),qGreen(abs(imX.pixel(x,y))),qBlue(abs(imX.pixel(x,y))));
                    imOut.setPixel(x,y,val);
                }
                else {
                    val = qRgb(0,0,0);
                    imOut.setPixel(x,y,val);
                }
            }
            else
            {
                a1 = imNorme.pixel(x,y-dy);
                a2 = imNorme.pixel(x+dx,y-dy);
                b1 = imNorme.pixel(x,y+dy);
                b2 = imNorme.pixel(x-dx,y+dy);
                A = (abs(imY.pixel(x,y)) - abs(imX.pixel(x,y)))*a1 + abs(imX.pixel(x,y))*a2;
                B = (abs(imY.pixel(x,y)) - abs(imX.pixel(x,y)))*b1 + abs(imX.pixel(x,y))*b2;
                point = imNorme.pixel(x,y) * abs(imY.pixel(x,y));
                if(point >= A && point > B) {
                    val = qRgb(qRed(abs(imY.pixel(x,y))),qGreen(abs(imY.pixel(x,y))),qBlue(abs(imY.pixel(x,y))));
                    imOut.setPixel(x,y,val);
                }
                else {
                    val = qRgb(0,0,0);
                    imOut.setPixel(x,y,val);
                }
            }
        }
    }
    return imOut;
}

QImage Filtres::seuillage(QImage imNorme, int seuil)
{
    QImage imOut = imNorme.copy(0,0,imNorme.width(),imNorme.height());//this->eclaircir(50,imNorme);
    for (int i=0 ; i<imOut.width() ; i++)
        for (int j=0; j<imOut.height() ; j++)
            if (qRed(imOut.pixel(i,j)) < seuil)
                imOut.setPixel(i,j,qRgb(0,0,0));
    return imOut;
}

QImage Filtres::decoupage_intelligent_contours(QImage imIn)
{
    qDebug()<<"fonction decoupage intelligent: image: "<<imIn.width()<<" ; "<<imIn.height();
    QImage imOut = this->norme_4gradients(GRADIENT_PREWITT,imIn);
    imOut = this->seuillage(imOut,25);

    int **pixels = new int *[imOut.width()];
    for (int i=0 ; i<imOut.width() ; i++)
        pixels[i] = new int[imOut.height()];

    for (int i=0 ; i<imOut.width() ; i++)
        for (int j=0; j<imOut.height() ; j++)
                pixels[i][j] = qRed(imOut.pixel(i,j));

    qDebug()<<"decoupage recursif coin haut gauche";
    pixels = this->decoupage_recursif(pixels,0,0,imOut.width(),imOut.height());
    qDebug()<<"decoupage recursif coin bas droite";
    pixels = this->decoupage_recursif(pixels,imOut.width()-1,imOut.height()-1,imOut.width(),imOut.height());
    qDebug()<<"decoupage recursif coin haut droite";
    pixels = this->decoupage_recursif(pixels,imOut.width()-1,0,imOut.width(),imOut.height());
    qDebug()<<"decoupage recursif coin bas gauche";
    pixels = this->decoupage_recursif(pixels,0,imOut.height()-1,imOut.width(),imOut.height());

    for (int i=0 ; i<imOut.width() ; i++)
        for (int j=0; j<imOut.height() ; j++)
        {
            if (pixels[i][j] == -1)
                imOut.setPixel(i,j,qRgba(255,0,0,0));
            else
                imOut.setPixel(i,j,imIn.pixel(i,j));
        }

    return imOut;
}

QImage Filtres::decoupage_intelligent_clic(QImage imIn, int x, int y)
{
    qDebug()<<"fonction decoupage intelligent: image: "<<imIn.width()<<" ; "<<imIn.height();
    QImage imOut = this->norme_4gradients(GRADIENT_PREWITT,imIn);
    imOut = this->seuillage(imOut,25);

    int **pixels = new int *[imOut.width()];
    for (int i=0 ; i<imOut.width() ; i++)
        pixels[i] = new int[imOut.height()];

    for (int i=0 ; i<imOut.width() ; i++)
        for (int j=0; j<imOut.height() ; j++)
                pixels[i][j] = qRed(imOut.pixel(i,j));

    pixels = this->decoupage_recursif(pixels,x,y,imOut.width(),imOut.height());

    for (int i=0 ; i<imOut.width() ; i++)
        for (int j=0; j<imOut.height() ; j++)
        {
            if (pixels[i][j] == -1)
                imOut.setPixel(i,j,qRgba(255,0,0,0));
            else
                imOut.setPixel(i,j,imIn.pixel(i,j));
        }

    return imOut;
}

int **Filtres::decoupage_recursif(int **pixels,int x, int y, int width, int height)
{

    //cas de debordement
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        //qDebug()<<"débordement: "<<x<<" , "<<y;
        return pixels;
    }
    //cas de base
    if (pixels[x][y] >0 || pixels[x][y] == -1)
    {
        //qDebug()<<"cas de base: "<<x<<" , "<<y;
        return pixels;
    }
    //recursion sur les pixels entourant le pixel courant
    //qDebug()<<"recursion: "<<x<<" , "<<y;
    pixels[x][y] = -1;
    //if (x-1 < 0 || x-1 >= width || y-1 < 0 || y-1 >= height)
        pixels = decoupage_recursif(pixels,x-1,y-1,width,height);
    //if (x-1 < 0 || x-1 >= width || y < 0 || y >= height)
        pixels = decoupage_recursif(pixels,x-1,y,width,height);
    //if (x-1 < 0 || x-1 >= width || y+1 < 0 || y+1 >= height)
        pixels = decoupage_recursif(pixels,x-1,y+1,width,height);
    //if (x < 0 || x >= width || y+1 < 0 || y+1 >= height)
        pixels = decoupage_recursif(pixels,x,y+1,width,height);
    //if (x+1 < 0 || x+1 >= width || y+1 < 0 || y+1 >= height)
        pixels = decoupage_recursif(pixels,x+1,y+1,width,height);
    //if (x+1 < 0 || x+1 >= width || y < 0 || y >= height)
        pixels = decoupage_recursif(pixels,x+1,y,width,height);
    //if (x+1 < 0 || x+1 >= width || y-1 < 0 || y-1 >= height)
        pixels = decoupage_recursif(pixels,x+1,y-1,width,height);
    //if (x < 0 || x >= width || y-1 < 0 || y-1 >= height)
        pixels = decoupage_recursif(pixels,x,y-1,width,height);

    return pixels;
}
