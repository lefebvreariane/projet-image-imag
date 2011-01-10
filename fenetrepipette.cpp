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

    labelR->setText("R:" + QString::number(r));
    labelG->setText("V:" + QString::number(g));
    labelB->setText("B:" + QString::number(b));

    float y, u, v;

    y = r*0.299 + g*0.587 + b*0.114;
    u = -0.14713*r - 0.28886*g + 0.436*b;
    v = 0.615*r - 0.51498*g -0.10001*b;

    labelY->setText("Y:" + QString::number((int) y));
    labelU->setText("U:" + QString::number((int) u));
    labelV->setText("V:" + QString::number((int) v));
}

