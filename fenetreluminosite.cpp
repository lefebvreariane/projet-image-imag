#include "fenetreluminosite.h"

FenetreLuminosite::FenetreLuminosite(QWidget *parent) :
        QWidget(parent)
{

    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label_lum = new QLabel("Luminosité: ");
    QLabel *label_cont = new QLabel("Contraste: ");

    label_val_lum = new QLabel("0");
    label_val_cont = new QLabel("0");

    slider_lum = new QSlider(Qt::Horizontal);
    slider_lum->setMinimum(-100);
    slider_lum->setMaximum(100);
    connect(slider_lum,SIGNAL(valueChanged(int)),this, SLOT(changement_lum(int)));

    slider_cont = new QSlider(Qt::Horizontal);
    slider_cont->setMinimum(-100);
    slider_cont->setMaximum(100);
    connect(slider_cont,SIGNAL(valueChanged(int)),this, SLOT(changement_cont(int)));

    //Creation du layout principal
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label_lum, 0, 0);
    layout->addWidget(slider_lum, 0, 1);
    layout->addWidget(label_val_lum, 0, 2);

    layout->addWidget(label_cont, 1, 0);
    layout->addWidget(slider_cont, 1, 1);
    layout->addWidget(label_val_cont, 1, 2);

    layout->addWidget(boutonOk, 2, 0);
    layout->addWidget(boutonAnnuler, 2, 1,1,2);

    setLayout(layout);
    this->setMaximumWidth(TAILLEH);
}

void FenetreLuminosite::changement_lum(int i){
    label_val_lum->setText(QString::number(i));
}

void FenetreLuminosite::changement_cont(int i){
    label_val_cont->setText(QString::number(i));
}


void FenetreLuminosite::clic_ok(){
    emit appliquer_luminosite_contraste((slider_lum->value()+100)/200.0,(slider_cont->value()+100)/200.0);
    this->hide();
}

void FenetreLuminosite::clic_annuler(){
    this->hide();
}

void FenetreLuminosite::init_lum_cont(){
    slider_lum->setValue(0);
    slider_cont->setValue(0);
}
