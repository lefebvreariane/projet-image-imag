#include <QtGui>
#include "fenetreredim.h"

FenetreRedim::FenetreRedim(QWidget *parent) :
        QWidget(parent)
{
    //Creation du layout principal
    QGridLayout *layoutRedim = new QGridLayout;
    QGroupBox  *groupParam = new QGroupBox("Paramètres");
    QGroupBox  *groupResum = new QGroupBox("Résumé") ;

    // Creation des boutons
    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));


    //Groupe parametre

    origine = new QRadioButton("Taille d'origine");
    origine->setChecked(true);
    connect(origine,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));

    //Ajout du rapport normalisé
    rapport = new QRadioButton("Rapport fixe");
    connect(rapport,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));

    spin_rapport = new QSpinBox();
    spin_rapport->setAccelerated(true);
    spin_rapport->setMinimum(1);
    spin_rapport->setMaximum(9999);

    spin_rapport->setSuffix("%");
    spin_rapport->setEnabled(false);
    connect(spin_rapport,SIGNAL(valueChanged(int)),this,SLOT(MAJ_valeurs_redim()));


    //Ajout du rapport personnalisé
    rapport_perso = new QRadioButton("Rapport personnalisé");
    connect(rapport_perso,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));

    spin_perso_larg = new QSpinBox();
    spin_perso_larg->setAccelerated(true);
    spin_perso_larg->setMinimum(1);
    spin_perso_larg->setMaximum(9999);

    spin_perso_larg->setSuffix("%");
    spin_perso_larg->setEnabled(false);
    connect(spin_perso_larg,SIGNAL(valueChanged(int)),this,SLOT(MAJ_valeurs_redim()));

    spin_perso_haut = new QSpinBox();
    spin_perso_haut->setAccelerated(true);
    spin_perso_haut->setMinimum(1);
    spin_perso_haut->setMaximum(9999);

    spin_perso_haut->setSuffix("%");
    spin_perso_haut->setEnabled(false);
    connect(spin_perso_haut,SIGNAL(valueChanged(int)),this,SLOT(MAJ_valeurs_redim()));

    //Ajout de la taille personnalisée
    taille_perso = new QRadioButton("Taille personnalisée");
    connect(taille_perso,SIGNAL(clicked()),this,SLOT(clic_radioBouton()));

    spin_taille_larg = new QSpinBox();
    spin_taille_larg->setAccelerated(true);
    spin_taille_larg->setMinimum(1);
    spin_taille_larg->setMaximum(9999);

    spin_taille_larg->setSuffix(" pixels");
    spin_taille_larg->setEnabled(false);
    connect(spin_taille_larg,SIGNAL(valueChanged(int)),this,SLOT(MAJ_valeurs_redim()));

    spin_taille_haut = new QSpinBox();
    spin_taille_haut->setAccelerated(true);
    spin_taille_haut->setMinimum(1);
    spin_taille_haut->setMaximum(9999);

    spin_taille_haut->setSuffix(" pixels");
    spin_taille_haut->setEnabled(false);
    connect(spin_taille_haut,SIGNAL(valueChanged(int)),this,SLOT(MAJ_valeurs_redim()));


    QGridLayout *layoutParam = new QGridLayout;
    layoutParam->addWidget(origine,0,0);
    layoutParam->addWidget(rapport,1,0);
    layoutParam->addWidget(spin_rapport,2,0);
    layoutParam->addWidget(rapport_perso,3,0);
    layoutParam->addWidget(spin_perso_larg,4,0);
    layoutParam->addWidget(spin_perso_haut,4,1);
    layoutParam->addWidget(taille_perso,5,0);
    layoutParam->addWidget(spin_taille_larg,6,0);
    layoutParam->addWidget(spin_taille_haut,6,1);

    //Application du layout des parametres
    groupParam->setLayout(layoutParam);

    //Cration et application du layout resumé
    QVBoxLayout *layoutResume= new QVBoxLayout;
    tailleOrigine = new QLabel;
    tailleRedim = new QLabel;

    layoutResume->addWidget(tailleOrigine);
    layoutResume->addWidget(tailleRedim);
    groupResum->setLayout(layoutResume);


    //application du layout principal
    layoutRedim->addWidget(groupParam,0,0,1,2);
    layoutRedim->addWidget(groupResum,1,0,1,2);
    layoutRedim->addWidget(boutonOk,2,0);
    layoutRedim->addWidget(boutonAnnuler,2,1);

    setLayout(layoutRedim);
}
void FenetreRedim::clic_radioBouton(){
    if (origine->isChecked()){
        spin_rapport->setEnabled(false);
        spin_perso_larg->setEnabled(false);
        spin_perso_haut->setEnabled(false);
        spin_taille_larg->setEnabled(false);
        spin_taille_haut->setEnabled(false);
    }

    else if (rapport->isChecked()){
        spin_rapport->setEnabled(true);
        spin_perso_larg->setEnabled(false);
        spin_perso_haut->setEnabled(false);
        spin_taille_larg->setEnabled(false);
        spin_taille_haut->setEnabled(false);
    }
    else if (rapport_perso->isChecked()){
        spin_rapport->setEnabled(false);
        spin_perso_larg->setEnabled(true);
        spin_perso_haut->setEnabled(true);
        spin_taille_larg->setEnabled(false);
        spin_taille_haut->setEnabled(false);
    }
    else if (taille_perso->isChecked()){
        spin_rapport->setEnabled(false);
        spin_perso_larg->setEnabled(false);
        spin_perso_haut->setEnabled(false);
        spin_taille_larg->setEnabled(true);
        spin_taille_haut->setEnabled(true);
    }

    MAJ_valeurs_redim();

}

void FenetreRedim::MAJ_valeurs_redim(){
    if (origine->isChecked()){
        largeur_redim = image.width();
        longueur_redim = image.height();
    }
    else if (rapport->isChecked()){
        largeur_redim = image.width()*spin_rapport->value()/100;
        longueur_redim = image.height()*spin_rapport->value()/100;
    }
    else if (rapport_perso->isChecked()){
        largeur_redim = image.width()*spin_perso_larg->value()/100;
        longueur_redim = image.height()*spin_perso_haut->value()/100;
    }
    else if (taille_perso->isChecked()){
        largeur_redim = spin_taille_larg->value();
        longueur_redim = spin_taille_haut->value();
    }
    MAJ_resume();
}

void FenetreRedim::MAJ_resume(){

    tailleOrigine->setText("Taille d'origine: " + QString::number(image.width()) + " x " + QString::number(image.height()) + " pixels" );
    tailleRedim->setText("Nouvelle taille: " + QString::number(largeur_redim) + " x " + QString::number(longueur_redim) + " pixels" );
}

void FenetreRedim::clic_ok(){

    this->hide();
}

void FenetreRedim::clic_annuler(){
    this->hide();
}

void FenetreRedim::initialiser(QImage i){
    image = i;
    tailleOrigine->setText("Taille d'origine: " + QString::number(image.width()) + " x " + QString::number(image.height()) + " pixels" );
    tailleRedim->setText("Nouvelle taille: " + QString::number(image.width()) + " x " + QString::number(image.height()) + " pixels" );
    spin_rapport->setValue(100);
    spin_perso_larg->setValue(100);
    spin_perso_haut->setValue(100);
    spin_taille_larg->setValue(image.width());
    spin_taille_haut->setValue(image.height());
}
