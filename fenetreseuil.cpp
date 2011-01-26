#include "fenetreseuil.h"

FenetreSeuil::FenetreSeuil(QWidget *parent) :
        QWidget(parent)
{


    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label= new QLabel("Seuil: ");
    label_val = new QLabel("0");

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(255);
    connect(slider,SIGNAL(valueChanged(int)),this, SLOT(changement_val(int)));

    //Creation du layout principal
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0);
    layout->addWidget(slider, 0, 1);
    layout->addWidget(label_val, 0, 2);
    layout->addWidget(boutonOk, 1, 0);
    layout->addWidget(boutonAnnuler, 1, 1,1,2);

    setLayout(layout);
    this->setMaximumWidth(TAILLEH);
}

void FenetreSeuil::changement_val(int i){
    label_val->setText(QString::number(i));
}


void FenetreSeuil::clic_ok(){
    emit appliquer_seuil(slider->value());
    this->hide();
}

void FenetreSeuil::clic_annuler(){
    this->hide();
}
void FenetreSeuil::init_seuil(){
    slider->setValue(125);
}


