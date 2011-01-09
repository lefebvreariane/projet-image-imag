#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>


enum Mode
{
    AUCUN, PIPETTE, HISTO, GREY, FLOU, FUSION
        };

class Controleur : public QObject
{
    Q_OBJECT
public:
    Controleur(ZoneDessin *zone);
    Mode mode;

public slots:
    void clic_recu();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);
    void RGB_to_grey();
    void appliquer_flou();
    QImage flouter_coins(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    QImage flouter_bords(QImage imIn, QImage imOut, int distPixel, int tailleFiltre);
    ZoneDessin *z;

};

#endif // CONTROLEUR_H
