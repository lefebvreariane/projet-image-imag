#ifndef FENETRELUMINOSITE_H
#define FENETRELUMINOSITE_H

#include <QtGui>

class FenetreLuminosite : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreLuminosite(QWidget *parent = 0);
    void init_lum_cont();

signals:
    void appliquer_luminosite_contraste(float, float);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_lum(int);
    void changement_cont(int);
private:

    static const int TAILLEH = 250;
    QSlider *slider_lum;
    QSlider *slider_cont;
    QLabel *label_val_lum;
    QLabel *label_val_cont;

};

#endif // FENETRELUMINOSITE_H
