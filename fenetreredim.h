#ifndef FENETREREDIM_H
#define FENETREREDIM_H

#include <QtGui>

class FenetreRedim : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreRedim(QWidget *parent = 0);

    void initialiser(QImage i);


signals:
    void redim(int, int);

public slots:

    void clic_ok();
    void clic_annuler();
    void clic_radioBouton();
    void MAJ_valeurs_redim();
    void MAJ_resume();

private:
    QImage image;
    int largeur_redim, hauteur_redim;
    QRadioButton *origine;
    QRadioButton *rapport;
    QRadioButton *rapport_perso;
    QRadioButton *taille_perso;
    QSpinBox *spin_rapport;
    QSpinBox *spin_perso_larg;
    QSpinBox *spin_perso_haut;
    QSpinBox *spin_taille_larg;
    QSpinBox *spin_taille_haut;
    QLabel *tailleOrigine;
    QLabel *tailleRedim;


};

#endif // FENETREREDIM_H
