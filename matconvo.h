#ifndef MATCONVO_H
#define MATCONVO_H

#include "noyaupascal.h"

enum TypeConvo
{
    GAUSS, MOYENNE, CONTOUR_LAPLACIEN, CONTOUR_SOBEL, NOYAU, GRADIENT_X, GRADIENT_Y
};

/*enum Dimension
{
    DIMENSION1, DIMENSION2
};*/


class MatConvo
{
private:
    int coef;
    int tFiltre;
    int *mat1;
    int **mat2;

public:
    MatConvo(int tailleFiltre, int coefficient);

    int getTFiltre();
    int getCoef();
    int getMat1(int i);
    int getMat2(int i, int j);

    void setMat1(int i, int val);
    void setMat2(int i, int j, int val);

    bool isNullMat1();
    bool isNullMat2();

    void noyau_gauss_bruit();
    void noyau_coef();
    void noyau_moyenne();
};

#endif // MATCONVO_H
