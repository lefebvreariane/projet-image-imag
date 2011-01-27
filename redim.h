#ifndef REDIM_H
#define REDIM_H
#include <QImage>
#include <QLinkedList>
#include "filtres.h"

struct Chemin {
    int *chemin;
    int n;
};


class Redim
{
public:
    Redim(QImage base);
    void maj(QImage img);
    QImage up(int l, int h);
    QImage down(int l, int h);
    QImage interpolationBilineaire(int l, int h);
    QImage redimSeamCarving(int l, int h);
    QImage decouper(int sX0, int sY0,int sX1,int sY1);

private:
    QImage image, imgGrad;
    int **matriceL, **matriceH;
    Filtres *f;
    bool pair;
    QLinkedList<Chemin> listeLigne;
    double fRed(int i, int j);
    double fGreen(int i, int j);
    double fBlue(int i, int j);
    double H(int j, double x, int c);
    QColor value(double x, double y);
    void evaluationScH();
    void evaluationScL();
    void majCheminH();
    void majCheminL();
    QImage redScUpL(QImage base);
    QImage redScUpH(QImage base);
    int min(int i1,int i2);
    int minM(int i1, int i2, int i3);
    int min3(int i1, int i2, int i3);
};

#endif // REDIM_H
