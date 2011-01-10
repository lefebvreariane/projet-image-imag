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
    void reInitSelection();

public slots:
    void clic_recu();
    QImage decouper();

signals:
    void afficher_pixel(int r, int g, int b);

private:
    void pipette(int x, int y);
    void RGB_to_grey();
    void appliquer_flou();
    int sX0, sX1, sY0, sY1;
    QPainter paint;
    ZoneDessin *z;

};

#endif // CONTROLEUR_H
