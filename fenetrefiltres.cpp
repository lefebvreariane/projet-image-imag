#include "fenetrefiltres.h"

FenetreFiltres::FenetreFiltres(QWidget *parent) :
        QWidget(parent)
{


    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));



    modele_matrice = new QStandardItemModel(3, 3);

    vue = new QTableView;
    vue->setModel(modele_matrice);

    vue->setItemDelegate(new TabFiltres);
    resize_taille(3);
    vue->setMaximumSize(222,225);
    vue->setMinimumSize(222,225);



    taille_matrice = new QLabel("Taille de la matrice: ");
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(1);
    slider->setMaximum(8);
    connect(slider,SIGNAL(valueChanged(int)),this, SLOT(changement_taille_matrice(int)));
    slider->setValue(2);

    QLabel *labelAlpha = new QLabel("Alpha:  ");
    spin_alpha = new QSpinBox();
    spin_alpha->setMinimum(1);
    spin_alpha->setMaximum(999);
    spin_alpha->setValue(1);


    QGridLayout *layoutFiltres = new QGridLayout;
    layoutFiltres->addWidget(taille_matrice,0,0,1,2);
    layoutFiltres->addWidget(slider,1,0,1,2);
    layoutFiltres->addWidget(vue,2,0,1,2);
    layoutFiltres->addWidget(labelAlpha,3,0);
    layoutFiltres->addWidget(spin_alpha,3,1);

    layoutFiltres->addWidget(boutonOk,4,0);
    layoutFiltres->addWidget(boutonAnnuler,4,1);


    setLayout(layoutFiltres);
    this->setMaximumWidth(TAILLEH);
    this->setMinimumWidth(TAILLEH);

}

void FenetreFiltres::clic_ok(){
    int taille = 2 * slider->value()-1;
    QModelIndex index;
    QVariant element;
    MatConvo *m = new MatConvo();
    m->allouerMem(taille,spin_alpha->value());
    for(int i =0; i<taille; i++){
        for(int j=0; j<taille; j++){
            index = modele_matrice->index(i,j);
            element = modele_matrice->data(index);
            m->ajouter_val_mat2(element.toInt());
        }
    }
    emit appliquer_filtre_perso(m);
    //m->~MatConvo();
    //this->hide();
}

void FenetreFiltres::clic_annuler(){
    this->hide();
}
void FenetreFiltres::changement_taille_matrice(int i){
    int taille = 2*i-1;
    taille_matrice->setText("Taille de la matrice: "+ QString::number(taille));

    resize_taille(taille);
}

void FenetreFiltres::resize_taille(int taille){
    modele_matrice->setColumnCount(taille);
    modele_matrice->setRowCount(taille);


    for (int i=0; i< taille; i++){
        if (taille <9){
            vue->setColumnWidth(i,TAILLE_MATRICE/taille);
            vue->setRowHeight(i,TAILLE_MATRICE/taille);
        }
        else {
            vue->setColumnWidth(i,TAILLE_MATRICE/9);
            vue->setRowHeight(i,TAILLE_MATRICE/9);
        }
    }
}
