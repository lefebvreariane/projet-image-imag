#ifndef NOYAUPASCAL_H
#define NOYAUPASCAL_H

class NoyauPascal
{
private:
    int taille;
    int coef;

public:
    // CONSTRUCTEUR
    NoyauPascal(int coefficient);

    // ACCESSEURS
    int getTaille();
    int getCoef();

    // MODIFICATEUR
    void setTaille(int val);

    // FONCTION
    void calcul_taille();
};

#endif // NOYAUPASCAL_H
