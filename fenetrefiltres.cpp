#include "fenetrefiltres.h"

FenetreFiltres::FenetreFiltres(QWidget *parent) :
    QWidget(parent)
{


    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));



}

void FenetreFiltres::clic_ok(){
    this->hide();
}

void FenetreFiltres::clic_annuler(){
    this->hide();
}
