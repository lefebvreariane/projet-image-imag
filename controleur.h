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
    SELECTION, PIPETTE, HISTO, GREY, MEDIAN, FLOU, FUSION, DECOUPAGE, REDIM, FILTRE, GRADIENT, REHAUSSEUR
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


    /*void seuillage(int seuil);
    void rehaussement_contraste();*/
    void appliquer_rehaussement(int alpha);
    void appliquer_laplacien(MatConvo *m);
    void afficher_histogrammes();
    void eclaircir(int alpha);
    void supp_non_maxima(TypeConvo tConv);
    void reInitSelection();


    ZoneDessin *z;
    Controleur(ZoneDessin *zone);
    Mode mode;



public slots:

    void clic_recu();
    void changer_mode(Mode);
    void redimensionner(int l, int h);
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

    QImage decouper();

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
