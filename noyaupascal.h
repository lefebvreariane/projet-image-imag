#ifndef NOYAUPASCAL_H
#define NOYAUPASCAL_H

class NoyauPascal
{
private:
    int taille;
    int coef;

public:
    NoyauPascal(int coefficient);

    int getTaille();
    int getCoef();

    void setTaille(int val);

    void calcul_taille();
};

#endif // NOYAUPASCAL_H
