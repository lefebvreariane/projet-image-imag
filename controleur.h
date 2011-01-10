#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>
#include <histogramme.h>


enum Mode
{
    SELECTION, PIPETTE, HISTO, GREY, FLOU, FUSION, DECOUPAGE
        };

class Controleur : public QObject
{
    Q_OBJECT
public:
    void RGB_to_grey();
    void appliquer_flou();
    void afficher_histogrammes();


    Controleur(ZoneDessin *zone);
    Mode mode;

public slots:
    void clic_recu();

    QImage decouper();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);

    QImage flouter_coins(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    QImage flouter_bords(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    ZoneDessin *z;
    Histogramme *histogramme;

};

#endif // CONTROLEUR_H
