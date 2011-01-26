#ifndef FENETREREHAUSSEUR_H
#define FENETREREHAUSSEUR_H

#include <QtGui>

class FenetreRehausseur : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreRehausseur(QWidget *parent = 0);

signals:
    void appliquer_rehausseur_laplacien(int, int);

public slots:
    void clic_ok();
    void clic_annuler();
    void changement_alpha(int);
private:

    static const int TAILLEH = 250;
    QComboBox *listeIntensite;
    QSlider *slider_alpha;
    QLabel *label_val_alpha;

};

#endif // FENETREREHAUSSEUR_H
