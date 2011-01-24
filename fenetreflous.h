#ifndef FENETREFLOUS_H
#define FENETREFLOUS_H
#include <QtGui>
#include "matconvo.h"

class FenetreFlous : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreFlous(QWidget *parent = 0);
    QRadioButton *Radio_lineaire;
    QRadioButton *Radio_gauss;
    QRadioButton *Radio_median;

signals:
void appliquer_flou(int, TypeConvo);
void appliquer_mediane(int);

public slots:
    void clic_ok();
    void clic_annuler();
     void clic_radioBouton();
    void changement_lineaire(int);
    void changement_gauss(int);
    void changement_median(int);

private:
    static const int TAILLEH = 250;
    void init_valeurs();
QGridLayout *layoutFlous;
    QLabel *label_lineaire;
    QLabel *label_gauss;
    QLabel *label_median;
    QSlider * slider_lineaire;
    QSlider * slider_gauss;
    QSlider * slider_median;

};

#endif // FENETREFLOUS_H
