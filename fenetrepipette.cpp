#include "fenetrepipette.h"
#include <QtGui>


FenetrePipette::FenetrePipette(QWidget *parent) :
        QWidget(parent)
{
    QGroupBox *pipette = new QGroupBox("Pipete", this);
    QGridLayout *layoutPipette = new QGridLayout;

    QGroupBox *groupRGB = new QGroupBox("RGB");
    layoutPipette->addWidget(groupRGB,0,0);
    QGroupBox *groupYUV = new QGroupBox("YUV");
    layoutPipette->addWidget(groupYUV,1,0);

    QGridLayout *layoutRGB = new QGridLayout;
    QGridLayout *layoutYUV = new QGridLayout;

    labelCouleur = new QLabel;
    labelR = new QLabel("Rouge: ");
    labelG = new QLabel("Vert : ");
    labelB = new QLabel("Bleu: ");
    layoutRGB->addWidget(labelR,0,0);
    layoutRGB->addWidget(labelG,1,0);
    layoutRGB->addWidget(labelB,2,0);

    labelY = new QLabel("Y: ");
    labelU = new QLabel("U: ");
    labelV = new QLabel("V: ");
    layoutYUV->addWidget(labelY,0,0);
    layoutYUV->addWidget(labelU,1,0);
    layoutYUV->addWidget(labelV,2,0);

    groupRGB->setLayout(layoutRGB);
    groupYUV->setLayout(layoutYUV);
    pipette->setLayout(layoutPipette);
}

void FenetrePipette:: afficher_pixel(int r, int g, int b){

    labelR->setText("R:" +QString::number(r));
    labelG->setText("V:" + QString::number(g));
    labelB->setText("B:" + QString::number(b));
    //labelY->setText("Rouge:" + r);
    //labelU->setText("Rouge:" + r);
    //labelV->setText("Rouge:" + r);
}

