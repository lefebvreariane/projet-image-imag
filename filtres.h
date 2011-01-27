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
    QImage eclaircir(int alpha, QImage imIn);
    QImage inverser_couleurs(QImage imIn);

    MatConvo *creer_filtre(int coefOuTaille, TypeConvo tConv);
    MatConvo *creer_laplacien(int numero, int alpha);
    MatConvo *creer_rehausseur_laplacien(int numero, int alpha);
    MatConvo *creer_impulsionnel();
    MatConvo *creer_gradient_x(TypeConvo tConv);
    MatConvo *creer_gradient_y(TypeConvo tConv);
    MatConvo *creer_gradient_moins_x(TypeConvo tConv);
    MatConvo *creer_gradient_moins_y(TypeConvo tConv);

    QImage norme_gradient(QImage imX, QImage imY);
    QImage norme_4gradients(TypeConvo tConv, QImage imIn);
    QImage passage_zero(int seuil, QImage laplacien);

    QImage *hysteresis(int seuilBas, int seuilHaut, QImage imContours);
    QImage chainage_contours(int seuilBas, int seuilHaut, TypeConvo tConv, QImage imIn);

    QImage appliquer_median(int taille, QImage imIn);
    QImage appliquer_flou(MatConvo *m, QImage imIn);
    QImage appliquer_rehaussement(int alpha, QImage imIn);
    QImage appliquer_laplacien(MatConvo *, QImage imIn);
    QImage appliquer_filtre(MatConvo *m, QImage imIn);

    //QImage orientation(QImage grad_horiz, QImage grad_vertical);
    QImage supp_non_maxima(QImage imX, QImage imY, QImage imNorme);
    /*QImage info_contours(QImage imIn);*/
    QImage seuillage(QImage imNorme, int seuil);

    QImage decoupage_intelligent_contours(QImage imIn);
    QImage decoupage_intelligent_clic(QImage imIn, int x, int y);
    QImage decoupage_intelligent(QImage imIn, int x, int y);
    int **decoupage_recursif(int **pixels,int x, int y, int width, int height);
};

#endif // FILTRES_H
