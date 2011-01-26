#include "fenetreflous.h"

FenetreFlous::FenetreFlous(QWidget *parent) :
        QWidget(parent)
{
    //Creation du layout principal
    QGridLayout *layoutFlous = new QGridLayout;



    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));


    Radio_lineaire = new QRadioButton("Flou linéaire");
    connect(Radio_lineaire,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));
    Radio_lineaire->setChecked(true);

    Radio_gauss = new QRadioButton("Flou gaussien");
    connect(Radio_gauss,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));

    Radio_median = new QRadioButton("Reduction de bruit");
    connect(Radio_median,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));



    label_lineaire = new QLabel;
    label_gauss = new QLabel;
    label_median = new QLabel;

    slider_lineaire = new QSlider(Qt::Horizontal);
    slider_lineaire->setMinimum(1);
    slider_lineaire->setMaximum(8);
    connect(slider_lineaire,SIGNAL(valueChanged(int)),this, SLOT(changement_lineaire(int)));

    slider_gauss = new QSlider(Qt::Horizontal);
    slider_gauss->setMinimum(1);
    slider_gauss->setMaximum(100);
    connect(slider_gauss,SIGNAL(valueChanged(int)),this, SLOT(changement_gauss(int)));

    slider_median = new QSlider(Qt::Horizontal);
    slider_median->setMinimum(1);
    slider_median->setMaximum(8);
    connect(slider_median,SIGNAL(valueChanged(int)),this, SLOT(changement_median(int)));

    init_valeurs();


    layoutFlous = new QGridLayout;
    layoutFlous->addWidget(Radio_lineaire, 0, 0);
    layoutFlous->addWidget(slider_lineaire, 0, 1);
    layoutFlous->addWidget(label_lineaire, 0, 2);
    layoutFlous->addWidget(Radio_gauss, 1, 0);
    layoutFlous->addWidget(slider_gauss, 1, 1);
    layoutFlous->addWidget(label_gauss, 1, 2);
    layoutFlous->addWidget(Radio_median, 2, 0);
    layoutFlous->addWidget(slider_median, 2, 1);
    layoutFlous->addWidget(label_median, 2, 2);
    layoutFlous->addWidget(boutonOk, 3, 0);
    layoutFlous->addWidget(boutonAnnuler, 3, 1,1,2);

    setLayout(layoutFlous);

    this->setMaximumWidth(TAILLEH);


}
void FenetreFlous::init_valeurs(){
    slider_lineaire->setValue(4);
    slider_gauss->setValue(50);
    slider_median->setValue(4);
    clic_radioBouton();

}

void FenetreFlous::changement_lineaire(int i){
    label_lineaire->setText(QString::number(i));
}
void FenetreFlous::changement_gauss(int i){
    label_gauss->setText(QString::number(i));
}
void FenetreFlous::changement_median(int i){
    label_median->setText(QString::number(i));
}

void FenetreFlous::clic_ok(){
    if (Radio_lineaire->isChecked()){
        emit appliquer_flou(slider_lineaire->value()*2 -1, MOYENNE);
    }
    else if (Radio_gauss->isChecked()){
        emit appliquer_flou(slider_gauss->value(),GAUSS);
    }
    else if (Radio_median->isChecked()){
        //GERER LE CAS DE LA COULEUR
        emit appliquer_mediane(slider_median->value()*2 -1);
    }

    this->hide();
}

void FenetreFlous::clic_annuler(){
    this->hide();
}

void FenetreFlous::clic_radioBouton(){
    if (Radio_lineaire->isChecked()){
        label_gauss->setEnabled(false);
        label_median->setEnabled(false);
        slider_gauss->setEnabled(false);
        slider_median->setEnabled(false);
        label_lineaire->setEnabled(true);
        slider_lineaire->setEnabled(true);
    }
    else if (Radio_gauss->isChecked()){
        label_lineaire->setEnabled(false);
        label_median->setEnabled(false);
        slider_lineaire->setEnabled(false);
        slider_median->setEnabled(false);
        label_gauss->setEnabled(true);
        slider_gauss->setEnabled(true);
    }
    else if (Radio_median->isChecked()){
        label_gauss->setEnabled(false);
        label_lineaire->setEnabled(false);
        slider_lineaire->setEnabled(false);
        slider_gauss->setEnabled(false);
        label_median->setEnabled(true);
        slider_median->setEnabled(true);
    }
}
