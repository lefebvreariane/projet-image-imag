#ifndef FENETREHISTOGRAMME_H
#define FENETREHISTOGRAMME_H

#include <QtGui>
#include <histogramme.h>


class FenetreHistogramme : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreHistogramme(QWidget *parent = 0);
    Histogramme *histogramme;

signals:
    void masquer_fenetre();

public slots:
    void changer_histo(int couche);
    void clic_ok();
    void clic_annuler();

private:


    QPushButton *boutonOk;
    QPushButton *boutonAnnuler;

};

#endif // FENETREHISTOGRAMME_H
