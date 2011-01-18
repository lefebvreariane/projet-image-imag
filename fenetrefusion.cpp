#include <QtGui>
#include "fenetrefusion.h"
#include "controleur.h"

FenetreFusion::FenetreFusion(QWidget *parent) :
        QFrame(parent)
{

    label_source = new QLabel(this);
    label_source->setMaximumSize(TAILLEH,TAILLEV);

    label_fusion = new BoutonOuverture(this);
    label_fusion->setMaximumSize(TAILLEH,TAILLEV);
    label_fusion->setMinimumSize(TAILLEH,TAILLEV);
    connect(label_fusion,SIGNAL(clic_bouton()), this, SLOT(open()) );
    //label_fusion->move(220,0);

    label_result = new QLabel(this);
    label_result->setMaximumSize(200,200);
    //label_fusion->move(420,0);

    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));


    layoutFusion = new QGridLayout;
    layoutFusion->addWidget(label_source, 0, 0, 1, 2);
    layoutFusion->addWidget(label_result, 2, 0, 1, 2);
    layoutFusion->addWidget(boutonOk, 4, 0);
    layoutFusion->addWidget(boutonAnnuler, 4, 1);

    setLayout(layoutFusion);





}

void FenetreFusion::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Ouvrir un fichier"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        image_fusion = QImage(fileName);
        if (image_fusion.isNull()) {
            QMessageBox::information(this, tr("ImageViewer"),
                                     tr("Imppossible d'ouvrir %1.").arg(fileName));
            return;
        }
    }

    label_fusion->setPixmap(QPixmap::fromImage(image_fusion.scaled(TAILLEH,TAILLEV,Qt::KeepAspectRatio)));
    label_fusion->adjustSize();
    layoutFusion->addWidget(label_fusion, 1, 0, 1, 2);

    image_result = image_source.copy();
    image_result = effectuer_fusion(image_source,image_fusion, image_result);
    emit changer_image(image_result);
    label_result->setPixmap(QPixmap::fromImage(image_result.scaled(TAILLEH,TAILLEV,Qt::KeepAspectRatio)));
    label_result->adjustSize();
}

void FenetreFusion::fusion(QImage i){
    image_source = i;
    label_source->setPixmap(QPixmap::fromImage(image_source.scaled(TAILLEH,TAILLEV,Qt::KeepAspectRatio)));
    label_source->adjustSize();

    /*open();
    label_fusion->setPixmap(QPixmap::fromImage(image_fusion));
    label_fusion->adjustSize();

    image_result = image_source.copy();
    image_result = effectuer_fusion(image_source,image_fusion, image_result);
    label_result->setPixmap(QPixmap::fromImage(image_result));
    label_result->adjustSize();
    */
}

void FenetreFusion::clic_ok(){
    finir_fusion(true);
}

void FenetreFusion::clic_annuler(){
    finir_fusion(false);

}

void FenetreFusion::finir_fusion(bool b){
    this->hide();

    if(!b) // annuler la fusion
        emit changer_image(image_source);
    else
        emit changer_image(image_result);

    emit changer_mode(SELECTION);
}


QImage FenetreFusion::effectuer_fusion(QImage src, QImage dest, QImage res)
{
    if(!image_source.isNull())

    {

        //charger l'image
        QPainter painter(&res);
        dest.convertToFormat(QImage::Format_ARGB32_Premultiplied,0);
        //----TRANSPARENCE----
        int i,j;
        for (i=0 ; i<dest.width() ; i++)
        {
            for(j=0 ; j<dest.height() ; j++)
            {
                QColor c = dest.pixel(i,j);
                //au lieu de 0.4 mettre la valeur d'opacite
                c.setAlphaF(c.alphaF() * 0.4);
                if (c.alpha()==0)
                    qDebug()<<"couleur=transparent";
                dest.setPixel(i,j,qRgba(c.red(),c.green(),c.blue(),c.alpha()));
            }
        }
        //----MODE DE COMPOSITION----
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        //appliquer l'image source a l'image courant au milieu de l'image
        painter.drawImage(src.width()/2 - res.width()/2,
                          src.height()/2 - res.height()/2, dest);
    }
    return res;
}




BoutonOuverture::BoutonOuverture(QWidget *parent) :
        QLabel(parent)
{

}

void BoutonOuverture::mousePressEvent(QMouseEvent *event)
{
    emit clic_bouton();
}

