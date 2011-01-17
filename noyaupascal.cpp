#include "noyaupascal.h"
#include <QtGui>

// CONSTRUCTEUR
NoyauPascal::NoyauPascal(int coefficient)
{
    this->taille = 0;
    this->coef = coefficient;
}

// ACCESSEURS
int NoyauPascal::getTaille()
{
    return this->taille;
}

int NoyauPascal::getCoef()
{
    return this->coef;
}

// MODIFICATEUR
void NoyauPascal::setTaille(int val)
{
    this->taille = val;
}

// FONCTION
void NoyauPascal::calcul_taille()
{
    qDebug()<< "fonction calcul_taille;";

    qDebug()<<"coefficient du noyau de Pascal: "<<getCoef();
    int coefP = getCoef();
    setTaille(1);
    while (coefP > 1)
    {
        coefP = (int) coefP/2;
        setTaille(getTaille()+1);
      qDebug()<<"taille = "<<getTaille()<<" ; coefP = "<<coefP;
    }
    setTaille(getTaille()*2 - 1);
    qDebug()<<"taille du noyau de pascal: "<<getTaille();
}
