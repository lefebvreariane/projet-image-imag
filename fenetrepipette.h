#ifndef FENETREPIPETTE_H
#define FENETREPIPETTE_H
#include <QtGui>

class FenetrePipette : public QWidget
{
    Q_OBJECT
public:
    explicit FenetrePipette(QWidget *parent = 0);
    QLabel *labelCouleur;
    QLabel *labelR;
    QLabel *labelG;
    QLabel *labelB;
    QLabel *labelH;
    QLabel *labelS;
    QLabel *labelV;




signals:
    void afficher_panneauDroite(bool);

public slots:

    void afficher_pixel(int r, int g, int b);

private:
QImage apercu;
/*
int min(int a, int b, int c);
int max(int a, int b, int c);
QColor*  RGBtoHSV(int var_R, int var_G, int var_B);
*/

};

#endif // FENETREPIPETTE_H
