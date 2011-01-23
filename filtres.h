#ifndef FILTRES_H
#define FILTRES_H

#include <QtGui>
#include "matconvo.h"
#include "noyaupascal.h"

class Filtres
{
public:
    Filtres();
    QImage RGB_to_grey(QImage imIn);
    MatConvo *creer_filtre(int coefOuTaille, TypeConvo tConv);
    MatConvo *creer_laplacien(int numero);
    MatConvo *creer_impulsionnel();
    QImage appliquer_median(int taille, QImage imIn);
    QImage appliquer_flou(MatConvo *m, QImage imIn);
    QImage info_contours(QImage imIn);
    QImage seuillage(int seuil, QImage imIn);
    QImage rehaussement_contraste(QImage imIn);
    QImage appliquer_rehaussement(int alpha, QImage imIn);
};

#endif // FILTRES_H
