#ifndef FENETREFILTRES_H
#define FENETREFILTRES_H

#include <QtGui>
#include "tabfiltres.h"
#include <matConvo.h>


class FenetreFiltres : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreFiltres(QWidget *parent = 0);

signals:
    void appliquer_filtre_perso(MatConvo*);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_taille_matrice(int);


private:
    static const int TAILLEH = 250;
    static const int TAILLE_MATRICE = 200;
    void resize_taille(int taille);

    QLabel *taille_matrice;
    QTableView *vue;
    QStandardItemModel *modele_matrice;
    QSlider *slider;
    QSpinBox *spin_alpha;
};

#endif // FENETREFILTRES_H
