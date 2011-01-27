#ifndef CONTROLEUR_H
#define CONTROLEUR_H
#include <QtGui>
#include <ZoneDessin.h>
#include <histogramme.h>
#include <matConvo.h>
#include "noyaupascal.h"
#include "filtres.h"



enum Mode
{
    SELECTION, PIPETTE, HISTO, GREY, MEDIAN, FLOU, FUSION,
    DECOUPAGE, DECOUPAGE_INTEL, REDIM, FILTRE, GRADIENT, REHAUSSEUR, LUMINOSITE,
    SEUIL, CONTOURS
};

class Controleur : public QObject
{
    Q_OBJECT
public:
    void RGB_to_grey();
    void inverser_couleurs();
    MatConvo *creer_filtre(int coefOuTaille, TypeConvo tconv);
    MatConvo *creer_laplacien(int numero, int alpha);
    MatConvo *creer_rehausseur_laplacien(int numero, int alpha);
    MatConvo *creer_impulsionnel();
    MatConvo *creer_gradient_x(TypeConvo tConv);
    MatConvo *creer_gradient_y(TypeConvo tConv);
    void passage_zero(int numero, int alpha, int seuil);

    void hysteresis(int seuilBas, int seuilHaut, int i);

    void appliquer_rehaussement(int alpha);
    void appliquer_laplacien(MatConvo *m);
    void afficher_histogrammes();
    void eclaircir(int alpha);
    void supp_non_maxima(TypeConvo tConv);
    void reInitSelection();
    void decoupage_intelligent_contours();
    void decoupage_intelligent_clic(int,int);

    void mon_seuillage(TypeConvo tConv, int seuil);
    ZoneDessin *z;
    Controleur(ZoneDessin *zone);
    Mode mode;



public slots:

    void clic_recu();
    void changer_mode(Mode);
    void redimensionner(int l, int h, int mode);
    void appliquer_median(int taille);
    void appliquer_flou(int taille,TypeConvo tConv);
    void appliquer_flou(MatConvo *m);
    void appliquer_filtre(MatConvo *m);
    void appliquer_gradient_moins_x(TypeConvo tConv);
    void appliquer_gradient_moins_y(TypeConvo tConv);
    void appliquer_gradient_x(TypeConvo tConv);
    void appliquer_gradient_y(TypeConvo tConv);
    void norme_gradient(TypeConvo tConv);
    void norme_4gradients(TypeConvo tConv);
    void appliquer_rehausseur_laplacien(int, int);
    void seuillage(int s);
    void luminosite_contraste(float lum, float cont);
    void appliquer_laplacien(int, int);
    void chainage_contours(int seuilBas, int seuilHaut, TypeConvo tConv);

    QImage decouper();
    void decoupage_intelligent_clic(int x, int y);

signals:
    void afficher_pixel(int r, int g, int b);
    void changer_message_barre(QString, int);

private:
    Filtres *f;
    void pipette(int x, int y);
    int sX0, sX1, sY0, sY1;
    QPainter paint;
    Histogramme *histogramme;

};

#endif // CONTROLEUR_H
