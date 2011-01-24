#include "fenetrepipette.h"

FenetrePipette::FenetrePipette(QWidget *parent) :
        QWidget(parent)
{
    QGroupBox *pipette = new QGroupBox("Pipette", this);
    QGridLayout *layoutPipette = new QGridLayout;

    labelCouleur = new QLabel;
    labelCouleur->setAlignment(Qt::AlignCenter);
    layoutPipette->addWidget(labelCouleur,0,0);
    apercu = QImage(20,20,QImage::Format_RGB32);

    QGroupBox *groupRGB = new QGroupBox("RGB");
    layoutPipette->addWidget(groupRGB,1,0);
    QGroupBox *groupHSV = new QGroupBox("HSV");
    layoutPipette->addWidget(groupHSV,2,0);

    QGridLayout *layoutRGB = new QGridLayout;
    QGridLayout *layoutHSV = new QGridLayout;

    labelR = new QLabel("Rouge: ");
    labelG = new QLabel("Vert : ");
    labelB = new QLabel("Bleu: ");
    layoutRGB->addWidget(labelR,0,0);
    layoutRGB->addWidget(labelG,1,0);
    layoutRGB->addWidget(labelB,2,0);

    labelH = new QLabel("H: ");
    labelS = new QLabel("S: ");
    labelV = new QLabel("V: ");
    layoutHSV->addWidget(labelH,0,0);
    layoutHSV->addWidget(labelS,1,0);
    layoutHSV->addWidget(labelV,2,0);

    groupRGB->setLayout(layoutRGB);
    groupHSV->setLayout(layoutHSV);
    pipette->setLayout(layoutPipette);
}

void FenetrePipette:: afficher_pixel(int r, int g, int b){

    labelR->setText("R:" +QString::number(r));
    labelG->setText("V:" + QString::number(g));
    labelB->setText("B:" + QString::number(b));

    float y, u, v;
    QPainter *painter= new QPainter(&apercu);
    painter->fillRect(0,0,20,20,QColor(r,g,b,255));
    painter->end();

    labelCouleur->setPixmap(QPixmap::fromImage(apercu));

    y = r*0.299 + g*0.587 + b*0.114;
    u = -0.14713*r - 0.28886*g + 0.436*b;
    v = 0.615*r - 0.51498*g -0.10001*b;


    QColor c= qRgb(r,g,b);


    labelH->setText("H:" +QString::number(c.hue()));
    labelS->setText("S:" +QString::number(c.saturation()));
    labelV->setText("V:" +QString::number( c.value()));

    emit(afficher_panneauDroite(true));
}
/*
QColor* FenetrePipette::RGBtoHSV(int var_R, int var_G, int var_B){

    QColor*  ret;
    int  H, S, V;

    //float var_R = ( R / 255 );
    //float var_G = ( G / 255 );
    //float var_B = ( B / 255 );

    int var_Min = min( var_R, var_G, var_B );
    int var_Max = max( var_R, var_G, var_B );
    int del_Max = var_Max - var_Min;

    V = var_Max;

    if ( del_Max == 0 )
    {
        H = 0;
        S = 0;
    }
    else
    {
        S = del_Max / var_Max;

        int del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
        int del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
        int del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max;

        if      ( var_R == var_Max ) H = del_B - del_G;
        else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B;
        else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R;

        if ( H < 0 ) H += 1;
        if ( H > 1 ) H -= 1;
    }

    qDebug()<<H;
    qDebug()<<S;
    qDebug()<<V;
    ret->setHsv(H,S,V,255);
    return ret;
}


int FenetrePipette::max(int a, int b, int c)
{
    if (a>b){
        if (a> c)
            return a;
        else return c;
    }
    else{
        if (b> c)
            return b;
        else return c;
    }
}



int FenetrePipette::min(int a, int b, int c)
{
    if (a<b){
        if (a< c)
            return a;
        else return c;
    }
    else{
        if (b< c)
            return b;
        else return c;
    }
}
*/
