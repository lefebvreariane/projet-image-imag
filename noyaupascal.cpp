#include "noyaupascal.h"
#include <QtGui>

NoyauPascal::NoyauPascal(int coefficient)
{
    this->taille = 0;
    this->coef = coefficient;
}

int NoyauPascal::getTaille()
{
    return this->taille;
}

int NoyauPascal::getCoef()
{
    return this->coef;
}

void NoyauPascal::setTaille(int val)
{
    this->taille = val;
}

void NoyauPascal::calcul_taille()
{
    qDebug()<<"coefficient du noyau de Pascal: "<<getCoef();
    int coefP = getCoef();
    setTaille(1);
    while (coefP != 1)
    {
        coefP = (int) getCoef()/2;
        setTaille(getTaille()+1);
    }
    setTaille(getTaille()*2 - 1);
    qDebug()<<"taille du noyau de pascal: "<<getTaille();
}
