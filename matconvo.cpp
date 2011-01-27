#include "matconvo.h"
#include "noyaupascal.h"
#include <QtGui>

// CONSTRUCTEUR
MatConvo::MatConvo()
{
    tCourante = 0;
}

// DESTRUCTEUR
MatConvo::~MatConvo()
{
    for (int i=0 ; i<getTFiltre() ; i++)
        delete mat2[i];
    delete mat2;
    delete mat1;
}

// ACCESSEURS
int MatConvo::getTFiltre()
{
    return this->tFiltre;
}

int MatConvo::getCoef()
{
    return this->coef;
}

int MatConvo::getMat1(int i)
{
    return this->mat1[i];
}

int MatConvo::getTCourante()
{
    return this->tCourante;
}

int MatConvo::getMat2(int i, int j)
{
    return this->mat2[i][j];
}

// MODIFICATEURS
void MatConvo::setTFiltre(int t)
{
    this->tFiltre = t;
}

void MatConvo::setCoef(int c)
{
    this->coef = c;
}

void MatConvo::setMat1(int i, int val)
{
    this->mat1[i] = val;
}

void MatConvo::setTCourante(int i)
{
    this->tCourante = i;
}

void MatConvo::inc_tCourante()
{
    this->tCourante++;
}

void MatConvo::setMat2(int i, int j, int val)
{
    this->mat2[i][j] = val;
}

// FONCTIONS BOOLEENNES
bool MatConvo::isNullMat1()
{
    return this->mat1==NULL;
}

bool MatConvo::isPleineMat1()
{
    return getTCourante() == getTFiltre();
}

bool MatConvo::isNullMat2()
{
    return this->mat2==NULL;
}

bool MatConvo::isPleineMat2()
{
    return getTCourante() == (getTFiltre()*getTFiltre());
}

// FONCTIONS
void MatConvo::allouerMem(int tailleFiltre, int coefficient)
{
    setTFiltre(tailleFiltre);
    setCoef(coefficient);

    this->mat1 = new int[this->tFiltre];
    this->mat2 = new int*[this->tFiltre];

    for (int i=0 ; i<this->tFiltre ; i++)
    {
        this->mat2[i] = new int[this->tFiltre];
    }
}

void MatConvo::noyau_gauss_bruit()
{

    if (isNullMat1())
    {
        return;
    }
    for (int i=0 ; i<getTFiltre() ; i++)
        for (int j=0 ; j<getTFiltre() ; j++)
            setMat2(i,j,getMat1(i)*getMat1(j));
}

void MatConvo::noyau_coef()
{

    // tMilieu-1 est l'indice du coef max
    int tMilieu = (int) getTFiltre()/2;
    tMilieu++;
    setMat1(tMilieu-1,getCoef());
    int coefP = getCoef();
    // si le noyau est unique...mais il ne le devrait pas
    if(getTFiltre() == 1)
    {
        return;
    }

    for(int i=tMilieu-2,j=tMilieu ; i>=0 && j<getTFiltre() ; i--,j++)
    {
        coefP = (int) coefP/2;
        setMat1(i,coefP);
        setMat1(j,coefP);
    }
}

void MatConvo::noyau_moyenne()
{
    // cree une matrice de convolution à 1, pour faire la moyenne sur les pixels
    for (int i=0 ; i<getTFiltre() ; i++)
    {
        for (int j=0 ; j<getTFiltre() ; j++)
            setMat2(i,j,1);
    }
}

void MatConvo::ajouter_val_mat1(int val)
{
    if (isPleineMat1())
    {
        return;
    }
    int i = 0,j;
    while (i<getTCourante() && val>= getMat1(i))
       i++;
    if (i != getTCourante())
    {
        for (j=getTCourante()-1 ; j>=i ; j--)
        {
            setMat1(j+1,getMat1(j));
        }
    }
    setMat1(i,val);
    inc_tCourante();
}

int MatConvo::gris_median(int med)
{
    return getMat1(med);
}

void MatConvo::ajouter_val_mat2(int val)
{
    if (isPleineMat2())
    {
        return;
    }

    setMat2((int) getTCourante()/getTFiltre(),getTCourante()%getTFiltre(),val);
    inc_tCourante();
}

void MatConvo::noyau_laplacien(int numero)
{
    switch (numero)
    {
    case 1:
        setMat2(0,0,0); setMat2(0,1,1); setMat2(0,2,0);
        setMat2(1,0,1); setMat2(1,1,-4); setMat2(1,2,1);
        setMat2(2,0,0); setMat2(2,1,1); setMat2(2,2,0);
        break;
    case 2:
        setMat2(0,0,1); setMat2(0,1,1); setMat2(0,2,1);
        setMat2(1,0,1); setMat2(1,1,-8); setMat2(1,2,1);
        setMat2(2,0,1); setMat2(2,1,1); setMat2(2,2,1);
        break;
    case 3:
        setMat2(0,0,1); setMat2(0,1,2); setMat2(0,2,1);
        setMat2(1,0,2); setMat2(1,1,-12); setMat2(1,2,2);
        setMat2(2,0,1); setMat2(2,1,2); setMat2(2,2,1);
        break;
    default:
        break;
    }
}

void MatConvo::noyau_impulsionnel()
{
    setMat2(0,0,0); setMat2(0,1,0); setMat2(0,2,0);
    setMat2(1,0,0); setMat2(1,1,1); setMat2(1,2,0);
    setMat2(2,0,0); setMat2(2,1,0); setMat2(2,2,0);
}

void MatConvo::gradient_x_lisse(TypeConvo tConv)
{
    if (tConv == GRADIENT_SOBEL)
    {
        setMat2(0,0,1); setMat2(0,1,0); setMat2(0,2,-1);
        setMat2(1,0,2); setMat2(1,1,0); setMat2(1,2,-2);
        setMat2(2,0,1); setMat2(2,1,0); setMat2(2,2,-1);
    }
    else if (tConv == GRADIENT_PREWITT)
    {
        setMat2(0,0,1); setMat2(0,1,0); setMat2(0,2,-1);
        setMat2(1,0,1); setMat2(1,1,0); setMat2(1,2,-1);
        setMat2(2,0,1); setMat2(2,1,0); setMat2(2,2,-1);
    }
}

void MatConvo::gradient_y_lisse(TypeConvo tConv)
{
    if (tConv == GRADIENT_SOBEL)
    {
        setMat2(0,0,1); setMat2(0,1,2); setMat2(0,2,1);
        setMat2(1,0,0); setMat2(1,1,0); setMat2(1,2,0);
        setMat2(2,0,-1); setMat2(2,1,-2); setMat2(2,2,-1);
    }
    else if (tConv == GRADIENT_PREWITT)
    {
        setMat2(0,0,1); setMat2(0,1,1); setMat2(0,2,1);
        setMat2(1,0,0); setMat2(1,1,0); setMat2(1,2,0);
        setMat2(2,0,-1); setMat2(2,1,-1); setMat2(2,2,-1);
    }
}
