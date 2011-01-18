#include "histogramme.h"
#include <QtGui>


Histogramme::Histogramme(QWidget *parent) :
        QWidget(parent)
{
    this->setMinimumSize(500,250);
    this->setMaximumSize(500,250);
    couche = 0; // Correspond au cas RGB
}

void Histogramme::paintEvent(QPaintEvent * event)
{
    //QFrame::paintEvent( event );
    //Appelle l'affichage du parent


    int largeur, hauteur, i;

    int *rouge = new int[256];
    int *vert = new int[256];
    int *bleu = new int[256];
    int *rgb = new int[256];

    QRgb p;

    /*Initilaisation des compteurs � 0*/
    for (largeur = 0; largeur <256; largeur++)
    {
        rouge[largeur] = 0;
        vert[largeur] = 0;
        bleu[largeur] = 0;
    }

    /*On compte le nombre de composante dans l'image pour chaque composante*/
    for (largeur =0; largeur< image.width(); largeur ++ )
    {
        for (hauteur =0; hauteur< image.height(); hauteur ++ )
        {
            p= image.pixel(largeur,hauteur);
            rouge[qRed(p)]++;
            vert[qGreen(p)]++;
            bleu[qBlue(p)]++;
        }
    }

    /*Calcul de la somme pour la plage de valeurs*/

    for (i=0; i< 256; i++)
    {
        rgb[i] = rouge[i]+ vert[i]+bleu[i];
    }
    /*calcul de l'histogramme a afficher*/
    switch (couche) {
    case 0: // RGB
        calcul_histo(rgb);
        break;
    case 1: // ROUGE
        calcul_histo(rouge);
        break;
    case 2: // VERT
        calcul_histo(vert);
        break;
    case 3: // BLEU
        calcul_histo(bleu);
        break;
    default:
        break;
    }



}

int Histogramme::maxi(int* tab, int taille)
{
    int i,indice, max=0;

    for (i=2; i<taille-2;i++)
        if (tab[i]>max){
        max = tab[i];
        indice = i;

    }
    return max;
}

void Histogramme::calcul_histo(int *tab){
    int i;
    int maximum = maxi(tab,256);

    QPainter painter(this) ;
    painter.setWindow(0,0,255,maximum);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);

    /*Affichage de l'histogramme*/
    for(i=0; i<256 ; i++)
    {
        painter.fillRect(1*i,maximum-tab[i],2,tab[i],Qt::red);
    }


    /*Affichage du repere*/
    painter.drawLine(0,0,0,maximum) ;
    painter.drawLine(0,maximum-1,255,maximum-1) ;
}

