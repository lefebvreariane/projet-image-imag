#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>
#include <histogramme.h>
#include <matConvo.h>
#include "filtres.h"


enum Mode
{
    SELECTION, PIPETTE, HISTO, GREY, MEDIAN, FLOU, FUSION, DECOUPAGE, REDIM
        };

class Controleur : public QObject
{
    Q_OBJECT
public:
    Controleur(ZoneDessin *zone);

    void RGB_to_grey();
    void inverser_couleurs();
    MatConvo *creer_filtre(int coefOuTaille, TypeConvo tconv);
    MatConvo *creer_laplacien(int numero);
    MatConvo *creer_impulsionnel();
    void appliquer_median(int taille);
    void appliquer_flou(MatConvo *);
    void seuillage(int seuil);
    void rehaussement_contraste();
    void appliquer_rehaussement(int alpha);

    void afficher_histogrammes();

    QImage redimensionner(int l, int h);
    void reInitSelection();

    Mode mode;
    ZoneDessin *z;

public slots:
    void clic_recu();
    void changer_mode(Mode);

    QImage decouper();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);

    int sX0, sX1, sY0, sY1;
    QPainter paint;

    Filtres *f;
    Histogramme *histogramme;

};

#endif // CONTROLEUR_H
