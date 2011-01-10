#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>


enum Mode
{
    SELECTION, PIPETTE, HISTO, GREY, FLOU, FUSION, DECOUPAGE
        };

class Controleur : public QObject
{
    Q_OBJECT
public:
    Controleur(ZoneDessin *zone);
    Mode mode;

public slots:
    void clic_recu();
    void RGB_to_grey();
    void appliquer_flou();
    QImage decouper();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);

    int sX0, sX1, sY0, sY1;
    QImage flouter_coins(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    QImage flouter_bords(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    QPainter paint;
    ZoneDessin *z;

};

#endif // CONTROLEUR_H
