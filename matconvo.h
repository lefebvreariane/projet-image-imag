#ifndef MATCONVO_H
#define MATCONVO_H

#include "noyaupascal.h"

enum TypeConvo
{
    GAUSS, MOYENNE, GRADIENT_SOBEL, GRADIENT_PREWITT
};

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
    bool isPleineMat2();

    // FONCTIONS
    void allouerMem(int tailleFiltre, int coefficient);
    void noyau_gauss_bruit();
    void noyau_coef();
    void noyau_moyenne();
    void ajouter_val_mat1(int val);
    int gris_median(int med);
    void ajouter_val_mat2(int val);
    void noyau_impulsionnel();
    void noyau_laplacien(int numero);
    void gradient_x_lisse(TypeConvo tConv);
    void gradient_y_lisse(TypeConvo tConv);

};

#endif // MATCONVO_H
