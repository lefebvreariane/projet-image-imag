#include "fenetregradient.h"

FenetreGradient::FenetreGradient(QWidget *parent) :
        QWidget(parent)
{
    //Creation du layout principal



    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label_type = new QLabel("Type de gradient: ");
    label_liste = new QLabel("Gradient: ");
    typeLaplacien = new QLabel("Type: ");
    labelLaplacien = new QLabel("Intensité: ");
    labelValLaplacien = new QLabel("0");



    listeGradient= new QComboBox();
    listeGradient->addItem("Gradient en x");
    listeGradient->addItem("Gradient en y");
    listeGradient->addItem("Gradient en -x");
    listeGradient->addItem("Gradient en -y");
    listeGradient->addItem("Norme de gradient");
    listeGradient->addItem("Norme de 4 gradients");

    listeTypeLaplacien = new QComboBox();
    listeTypeLaplacien->addItem("Faible");
    listeTypeLaplacien->addItem("Moyen");
    listeTypeLaplacien->addItem("Fort");

    sliderLaplacien = new QSlider(Qt::Horizontal);
    sliderLaplacien->setMinimum(0);
    sliderLaplacien->setMaximum(15);
    connect(sliderLaplacien,SIGNAL(valueChanged(int)),this, SLOT(changement_slider(int)));
    sliderLaplacien->setValue(7);

    typeGradient = new QComboBox();
    typeGradient->addItem("Sobel");
    typeGradient->addItem("Prewitt");
    typeGradient->addItem("Laplacien");
    connect(typeGradient,SIGNAL(currentIndexChanged(int)),this,SLOT(changement_type(int)));
    changement_type(0);


    QGridLayout *layoutGrad = new QGridLayout;
    layoutGrad->setColumnMinimumWidth(2,20);
    layoutGrad->addWidget(label_type, 0, 0);
    layoutGrad->addWidget(typeGradient, 0, 1);
    layoutGrad->addWidget(label_liste, 1, 0);
    layoutGrad->addWidget(listeGradient, 1, 1);
    layoutGrad->addWidget(typeLaplacien, 2, 0);
    layoutGrad->addWidget(listeTypeLaplacien, 2, 1);
    layoutGrad->addWidget(labelLaplacien, 3, 0);
    layoutGrad->addWidget(sliderLaplacien, 3, 1);
    layoutGrad->addWidget(labelValLaplacien, 3, 2);
    layoutGrad->addWidget(boutonOk, 4, 0);
    layoutGrad->addWidget(boutonAnnuler,4, 1);

    setLayout(layoutGrad);
    this->setMaximumWidth(TAILLEH);

}

void FenetreGradient::clic_ok(){
    TypeConvo t;
    if (typeGradient->currentIndex() == 0 )
        t = GRADIENT_SOBEL;
    else if (typeGradient->currentIndex() == 1 )
        t = GRADIENT_PREWITT;
    else
        t= LAPLACIEN;


    if (t != LAPLACIEN){
        switch (listeGradient->currentIndex()){
        case 0:{
                emit appliquer_gradient_x(t);
                break;
            }
        case 1:{
                emit appliquer_gradient_y(t);
                break;
            }
        case 2:{
                emit appliquer_gradient_moins_x(t);
                break;
            }
        case 3:{
                emit appliquer_gradient_moins_y(t);
                break;
            }
        case 4:{
                emit norme_gradient(t);
                break;
            }
        case 5:{
                emit norme_4gradients(t);
                break;
            }
        }
    }
    else{
        emit appliquer_laplacien(listeTypeLaplacien->currentIndex()+1,sliderLaplacien->value());
    }

    this->hide();
}

void FenetreGradient::clic_annuler(){
    this->hide();
}

void FenetreGradient::changement_type(int index){
    if (index != 2){
        typeLaplacien->setEnabled(false);
        listeTypeLaplacien->setEnabled(false);
        sliderLaplacien->setEnabled(false);
        labelLaplacien->setEnabled(false);
        labelValLaplacien->setEnabled(false);
        label_liste->setEnabled(true);
        listeGradient->setEnabled(true);
    }
    else{ //LAPLACIEN
        typeLaplacien->setEnabled(true);
        listeTypeLaplacien->setEnabled(true);
        sliderLaplacien->setEnabled(true);
        labelLaplacien->setEnabled(true);
        labelValLaplacien->setEnabled(true);
        label_liste->setEnabled(false);
        listeGradient->setEnabled(false);
    }
}

void FenetreGradient::changement_slider(int i){
    labelValLaplacien->setText(QString::number(i));
}
