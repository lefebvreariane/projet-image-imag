#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>
#include <histogramme.h>
#include <matConvo.h>
#include "noyaupascal.h"


enum Mode
{
    SELECTION, PIPETTE, HISTO, GREY, MEDIAN, FLOU, FUSION, DECOUPAGE, REDIM
        };

class Controleur : public QObject
{
    Q_OBJECT
public:
    void RGB_to_grey();
    MatConvo *creer_filtre(int coefOuTaille, TypeConvo tconv);
    void appliquer_median(int taille);
    void appliquer_flou(MatConvo *);
    void afficher_histogrammes();

    ZoneDessin *z;
    Controleur(ZoneDessin *zone);
    Mode mode;
    void reInitSelection();

public slots:
    void clic_recu();
    void changer_mode(Mode);
    void redimensionner(int l, int h);

    QImage decouper();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);
    int sX0, sX1, sY0, sY1;
    QPainter paint;


    Histogramme *histogramme;

};

#endif // CONTROLEUR_H
