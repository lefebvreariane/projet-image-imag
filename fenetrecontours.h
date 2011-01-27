#ifndef FENETRECONTOURS_H
#define FENETRECONTOURS_H

#include <QtGui>
#include "matconvo.h"

class FenetreContours : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreContours(QWidget *parent = 0);

signals:
    void detection_contours(int, int,TypeConvo);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_slider_bas(int);
    void changement_slider_haut(int);
private:

    static const int TAILLEH = 250;

    QComboBox *typeGradient;
    QSlider *slider_seuilBas;
    QSlider *slider_seuilHaut;
    QLabel *label_seuilBas_val;
    QLabel *label_seuilHaut_val;

};

#endif // FENETRECONTOURS_H
