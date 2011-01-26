#include "fenetregradient.h"

FenetreGradient::FenetreGradient(QWidget *parent) :
        QWidget(parent)
{
    //Creation du layout principal
    QGridLayout *layoutFlous = new QGridLayout;



    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));

    QLabel *label_type = new QLabel("Type de gradient: ");
    QLabel *label_liste = new QLabel("Gradient: ");

    typeGradient = new QComboBox();
    typeGradient->addItem("Sobel");
    typeGradient->addItem("Prewitt");

    listeGradient= new QComboBox();
    listeGradient->addItem("Gradient en x");
    listeGradient->addItem("Gradient en y");
    listeGradient->addItem("Gradient en -x");
    listeGradient->addItem("Gradient en -y");
    listeGradient->addItem("Norme de gradient");
    listeGradient->addItem("Norme de 4 gradients");


    layoutFlous = new QGridLayout;
    layoutFlous->addWidget(label_type, 0, 0);
    layoutFlous->addWidget(typeGradient, 0, 1);
    layoutFlous->addWidget(label_liste, 1, 0);
    layoutFlous->addWidget(listeGradient, 1, 1);
    layoutFlous->addWidget(boutonOk, 2, 0);
    layoutFlous->addWidget(boutonAnnuler, 2, 1);

    setLayout(layoutFlous);
    this->setMaximumWidth(TAILLEH);





}

void FenetreGradient::clic_ok(){
    TypeConvo t;

    if (typeGradient->currentIndex()== 0 )
        t = GRADIENT_SOBEL;
    else
        t = GRADIENT_PREWITT;

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

    this->hide();
}

void FenetreGradient::clic_annuler(){
    this->hide();
}
