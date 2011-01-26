#ifndef FENETREGRADIENT_H
#define FENETREGRADIENT_H

#include <QtGui>
#include "matconvo.h"


class FenetreGradient : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreGradient(QWidget *parent = 0);

signals:

    void appliquer_gradient_moins_x(TypeConvo tConv);
    void appliquer_gradient_moins_y(TypeConvo tConv);
    void appliquer_gradient_x(TypeConvo tConv);
    void appliquer_gradient_y(TypeConvo tConv);
    void norme_gradient(TypeConvo tConv);
    void norme_4gradients(TypeConvo tConv);
    void appliquer_laplacien(int, int);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_type(int);
    void changement_slider(int);
private:

    static const int TAILLEH = 250;
    QComboBox *typeGradient;
    QComboBox *listeGradient;
    QLabel *label_liste;

    QLabel *typeLaplacien;
    QComboBox *listeTypeLaplacien;
    QSlider *sliderLaplacien;
    QLabel *labelLaplacien;
    QLabel *labelValLaplacien;

};

#endif // FENETREGRADIENT_H
