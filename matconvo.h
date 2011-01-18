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
    int tCourante;
    int **mat2;

public:
    // CONSTRUCTEUR
    MatConvo();

    // DESTRUCTEUR
    ~MatConvo();

    // ACCESSEURS
    int getTFiltre();
    int getCoef();
    int getMat1(int i);
    int getTCourante();
    int getMat2(int i, int j);

    // MODIFICATEURS
    void setTFiltre(int t);
    void setCoef(int c);
    void setMat1(int i, int val);
    void setTCourante(int i);
    void inc_tCourante();
    void setMat2(int i, int j, int val);

    // FONCTIONS BOOLEENNES
    bool isNullMat1();
    bool isPleineMat1();
    bool isNullMat2();

    // FONCTIONS
    void allouerMem(int tailleFiltre, int coefficient);
    void noyau_gauss_bruit();
    void noyau_coef();
    void noyau_moyenne();
    void ajouter_gris(int gris);
    int gris_median(int med);

};

#endif // MATCONVO_H
