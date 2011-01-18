#include <QtGui>
#include "fenetrehistogramme.h"

FenetreHistogramme::FenetreHistogramme(QWidget *parent) :
        QWidget(parent)
{

    histogramme = new Histogramme(this);

    this->setMinimumSize(600,400);
    this->setMaximumSize(600,400);
    QLabel *couche = new QLabel("Couche: ",this);
    QComboBox *liste = new QComboBox(this);
    liste->addItem("RGB");
    liste->addItem("Rouge");
    liste->addItem("Vert");
    liste->addItem("Bleu");
    connect(liste,SIGNAL(currentIndexChanged(int)),this, SLOT(changer_histo(int)));

    QPushButton *boutonOk = new QPushButton("Ok", this);
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler",this);
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    couche->move(55,15);
    liste->move(100,10);
    histogramme->move(50,50);
    boutonOk->move(200,360);
    boutonAnnuler->move(300,360);


}

void FenetreHistogramme::changer_histo(int couche){
    qDebug()<<couche;
    histogramme->couche = couche;
    histogramme->repaint();
}


void FenetreHistogramme::clic_ok(){
    emit masquer_fenetre();
}

void FenetreHistogramme::clic_annuler(){
    emit masquer_fenetre();
}
