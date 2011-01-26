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

public slots:
    void clic_ok();
    void clic_annuler();
private:

    static const int TAILLEH = 250;
    QComboBox *typeGradient;
    QComboBox *listeGradient;

};

#endif // FENETREGRADIENT_H
