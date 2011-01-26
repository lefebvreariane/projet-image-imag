#ifndef FENETRESEUIL_H
#define FENETRESEUIL_H

#include <QtGui>

class FenetreSeuil : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreSeuil(QWidget *parent = 0);
    void init_seuil();

signals:
    void appliquer_seuil(int);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_val(int);
private:

    static const int TAILLEH = 250;
    QSlider *slider;
    QLabel *label_val;

};
#endif // FENETRESEUIL_H
