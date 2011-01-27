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
    int coefP = getCoef();
    setTaille(1);
    while (coefP > 1)
    {
        coefP = (int) coefP/2;
        setTaille(getTaille()+1);
    }
    setTaille(getTaille()*2 - 1);
}
