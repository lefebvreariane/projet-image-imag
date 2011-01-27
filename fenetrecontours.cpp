#include "fenetrecontours.h"

FenetreContours::FenetreContours(QWidget *parent) :
        QWidget(parent)

{
    //Creation du layout principal
    QGridLayout *layoutCont = new QGridLayout;

    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label_typeGradient = new QLabel("Type de gradient: ");
    QLabel *label_seuilBas = new QLabel("Seuil bas: ");
    QLabel *label_seuilHaut = new QLabel("Seuil haut: ");
    label_seuilBas_val = new QLabel("0");
    label_seuilHaut_val = new QLabel("0");


    typeGradient= new QComboBox();
    typeGradient->addItem("Sobel");
    typeGradient->addItem("Prewitt");
    typeGradient->addItem("Laplacien");

    slider_seuilBas = new QSlider(Qt::Horizontal);
    slider_seuilBas->setMinimum(0);
    slider_seuilBas->setMaximum(255);
    slider_seuilBas->setValue(125);
    connect(slider_seuilBas,SIGNAL(valueChanged(int)),this, SLOT(changement_slider_bas(int)));
    changement_slider_bas(125);

    slider_seuilHaut = new QSlider(Qt::Horizontal);
    slider_seuilHaut->setMinimum(0);
    slider_seuilHaut->setMaximum(255);
    slider_seuilHaut->setValue(125);
    connect(slider_seuilHaut,SIGNAL(valueChanged(int)),this, SLOT(changement_slider_haut(int)));
    changement_slider_haut(125);

    layoutCont = new QGridLayout;
    layoutCont->addWidget(label_typeGradient, 0, 0);
    layoutCont->addWidget(typeGradient, 0, 1);
    layoutCont->addWidget(label_seuilBas, 1, 0);
    layoutCont->addWidget(slider_seuilBas, 1, 1);
    layoutCont->addWidget(label_seuilBas_val, 1, 2);
    layoutCont->addWidget(label_seuilHaut, 2, 0);
    layoutCont->addWidget(slider_seuilHaut, 2, 1);
    layoutCont->addWidget(label_seuilHaut_val, 2, 2);
    layoutCont->addWidget(boutonOk, 3, 0);
    layoutCont->addWidget(boutonAnnuler,3, 1);

    setLayout(layoutCont);
    this->setMaximumWidth(TAILLEH);

}

void FenetreContours::clic_ok(){
    TypeConvo t;
    if (typeGradient->currentIndex() == 0 )
        t = GRADIENT_SOBEL;
    else if (typeGradient->currentIndex() == 1 )
        t = GRADIENT_PREWITT;
    else
        t= LAPLACIEN;

    emit detection_contours(slider_seuilBas->value(),slider_seuilHaut->value(),t);


    this->hide();
}

void FenetreContours::clic_annuler(){
    this->hide();
}

void FenetreContours::changement_slider_bas(int i){
    label_seuilBas_val->setText(QString::number(i));

}

void FenetreContours::changement_slider_haut(int i){
    label_seuilHaut_val->setText(QString::number(i));
}
