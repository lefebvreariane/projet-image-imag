#include "fenetrerehausseur.h"

FenetreRehausseur::FenetreRehausseur(QWidget *parent) :
        QWidget(parent)
{

    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label_intensite = new QLabel("Intensité: ");
    QLabel *label_alpha = new QLabel("Alpha: ");
    label_val_alpha = new QLabel("0");

    listeIntensite = new QComboBox();
    listeIntensite->addItem("Faible");
    listeIntensite->addItem("Moyen");
    listeIntensite->addItem("Fort");

    slider_alpha = new QSlider(Qt::Horizontal);
    slider_alpha->setMinimum(1);
    slider_alpha->setMaximum(10);
    connect(slider_alpha,SIGNAL(valueChanged(int)),this, SLOT(changement_alpha(int)));

    //Creation du layout principal
    QGridLayout *layoutRehausseur = new QGridLayout;
    layoutRehausseur->addWidget(label_intensite, 0, 0);
    layoutRehausseur->addWidget(listeIntensite, 0, 1);
    layoutRehausseur->addWidget(label_alpha, 1, 0);
    layoutRehausseur->addWidget(slider_alpha, 1, 1);
    layoutRehausseur->addWidget(label_val_alpha, 1, 2);
    layoutRehausseur->addWidget(boutonOk, 2, 0);
    layoutRehausseur->addWidget(boutonAnnuler, 2, 1,1,2);

    setLayout(layoutRehausseur);
    this->setMaximumWidth(TAILLEH);
}

void FenetreRehausseur::changement_alpha(int i){
    label_val_alpha->setText(QString::number(i));
}


void FenetreRehausseur::clic_ok(){
    emit appliquer_rehausseur_laplacien(listeIntensite->currentIndex()+1,slider_alpha->value());
    this->hide();
}

void FenetreRehausseur::clic_annuler(){
    this->hide();
}
