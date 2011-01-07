#ifndef CONTROLER_H
#define CONTROLER_H

#include <ZoneDessin.h>
#include <QWidget>
#include <QObject>

enum Mode
{
    AUCUN, PIPETTE,HISTO,GREY,FLOU,FUSION
};


class Controler : public QObject
{
    Q_OBJECT

public:
    Controler(ZoneDessin *zone);
    Mode mode;

public slots:

    void clic_recu();

private:
    ZoneDessin *z;
};

#endif // CONTROLER_H
