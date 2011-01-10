#include "histogramme.h"
#include <QtGui>


Histogramme::Histogramme(QImage i,QWidget *parent) :
        QWidget(parent)
{
    image = i;
}

void Histogramme::paintEvent(QPaintEvent * event)
{
    int largeur, hauteur, i, maximum;

    int *rouge = new int[256];
    int *vert = new int[256];
    int *bleu = new int[256];
    int *rgb = new int[256];

    QRgb p;
    for (largeur = 0; largeur <256; largeur++)
    {
        rouge[largeur] = 0;
        vert[largeur] = 0;
        bleu[largeur] = 0;
    }
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
    for (i=1; i< 256; i++)
    {
        rgb[i] = rouge[i]+ vert[i]+bleu[i];
    }
    maximum = maxi(rgb,256);

    QPainter painter(this) ;
    painter.setWindow(-10,-10,255,maximum);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawLine(0,0,0,maximum) ;
    painter.drawLine(0,0,255,0) ;
    for(i=1; i<256 ; i++)
    {
        painter.drawLine(i-1,rgb[i-1],i,rgb[i]);
    }
}

int Histogramme::maxi(int* tab, int taille)
{
    int i, max=0;

    for (i=0; i<taille;i++)
        if (tab[i]>max)
            max = tab[i];
    return max;
}
