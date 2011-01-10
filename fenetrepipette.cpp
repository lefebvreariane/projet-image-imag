#include "fenetrepipette.h"
#include <QtGui>


FenetrePipette::FenetrePipette(QWidget *parent) :
        QWidget(parent)
{
    QGroupBox *pipette = new QGroupBox("Pipete", this);
    QGridLayout *layoutPipette = new QGridLayout;

    labelCouleur = new QLabel;
    layoutPipette->addWidget(labelCouleur,0,0);
    qDebug()<<"OOOOO";
    apercu = QImage(20,20,QImage::Format_RGB32);

    qDebug()<<"11111";


    QGroupBox *groupRGB = new QGroupBox("RGB");
    layoutPipette->addWidget(groupRGB,1,0);
    QGroupBox *groupYUV = new QGroupBox("YUV");
    layoutPipette->addWidget(groupYUV,2,0);

    QGridLayout *layoutRGB = new QGridLayout;
    QGridLayout *layoutYUV = new QGridLayout;



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

    QPainter *painter= new QPainter(&apercu);
    painter->fillRect(0,0,20,20,QColor(r,g,b,255));
    painter->end();

    labelCouleur->setPixmap(QPixmap::fromImage(apercu));
    //labelY->setText("Rouge:" + r);
    //labelU->setText("Rouge:" + r);
    //labelV->setText("Rouge:" + r);

    emit(afficher_panneauDroite(true));
}

