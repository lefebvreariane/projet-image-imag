#include "fenetrefusion.h"
#include "controleur.h"

FenetreFusion::FenetreFusion(QWidget *parent) :
        QFrame(parent)
{
    this->setMaximumWidth(TAILLEH+30);

    label_source = new QLabel();
    label_source->setMaximumSize(TAILLEH,TAILLEV);

    label_fusion = new BoutonOuverture();
    connect(label_fusion,SIGNAL(clic_bouton()), this, SLOT(open()) );


    QPushButton *boutonOk = new QPushButton("Ok");
    connect(boutonOk, SIGNAL(clicked()),this, SLOT(clic_ok()));
    QPushButton *boutonAnnuler = new QPushButton("Annuler");
    connect(boutonAnnuler, SIGNAL(clicked()),this, SLOT(clic_annuler()));


    QLabel *label_mode = new QLabel("Mode de fusion: ");
    QComboBox *liste = new QComboBox(this);
    liste->addItem("Source");
    liste->addItem("Destination");
    liste->addItem("Source Over");
    liste->addItem("Destination Over");
    liste->addItem("Source In");
    liste->addItem("Destination In");
    liste->addItem("Source Out");
    liste->addItem("Destination Out");
    liste->addItem("Source Atop");
    liste->addItem("Destination Atop");
    liste->addItem("Clear");
    liste->addItem("XOR");
    connect(liste,SIGNAL(currentIndexChanged(int)),this, SLOT(changement_liste(int)));

    type_fusion = SOURCE;



    QLabel *label_transp = new QLabel("Transparence: ");
    label_transp_pourcentage = new QLabel;
    tansparence = new QSlider(Qt::Horizontal);
    tansparence->setMinimum(0);
    tansparence->setMaximum(100);
    tansparence->setValue(50);
    label_transp_pourcentage->setText(QString::number(tansparence->value()) + "%");
    connect(tansparence,SIGNAL(valueChanged(int)),this, SLOT(changement_tansparence(int)));
    transp_fusion = tansparence->value()/100.0;

    QHBoxLayout *layoutTransp = new QHBoxLayout;
    layoutTransp->addWidget(label_transp);
    layoutTransp->addWidget(tansparence);
    layoutTransp->addWidget(label_transp_pourcentage);


    layoutFusion = new QGridLayout;
    layoutFusion->addWidget(label_source, 0, 0, 1, 2);
    layoutFusion->addWidget(label_fusion, 1, 0, 1, 2);
    layoutFusion->addWidget(label_mode, 3, 0);
    layoutFusion->addWidget(liste, 3, 1);
    layoutFusion->addLayout(layoutTransp, 4, 0, 1, 2);
    layoutFusion->addWidget(boutonOk, 5, 0);
    layoutFusion->addWidget(boutonAnnuler, 5, 1);

    setLayout(layoutFusion);
}

void FenetreFusion::changement_liste(int i){
    switch (i){
    case 0: {
            type_fusion = SOURCE;
            break;
        }
    case 1:{
            type_fusion = DESTINATION;
            break;
        }
    case 2:{
            type_fusion =SOURCE_OVER ;
            break;
        }
    case 3:{
            type_fusion = DESTINATION_OVER;
            break;
        }
    case 4: {
            type_fusion =SOURCE_IN ;
            break;
        }
    case 5: {
            type_fusion = DESTINATION_IN;
            break;
        }
    case 6: {
            type_fusion =SOURCE_OUT ;
            break;
        }
    case 7: {
            type_fusion = DESTINATION_OUT;
            break;
        }
    case 8: {
            type_fusion =SOURCE_ATOP ;
            break;
        }
    case 9: {
            type_fusion =DESTINATION_ATOP ;
            break;
        }
    case 10: {
            type_fusion =CLEAR ;
            break;
        }
    case 11: {
            type_fusion =XOR ;
            break;
        }
    }


    image_result = effectuer_fusion(image_fusion,image_source, image_result,type_fusion,transp_fusion,0,0);
    emit changer_image_sans_save(image_result);

}


void FenetreFusion:: changement_tansparence(int i){

    transp_fusion = i/100.0;
    label_transp_pourcentage->setText(QString::number(tansparence->value()) + "%");
    image_result = effectuer_fusion(image_fusion,image_source, image_result,type_fusion,transp_fusion,0,0);
    emit changer_image_sans_save(image_result);
}



void FenetreFusion::initFusion(QImage i){
    image_source = i;

    label_source->setPixmap(QPixmap::fromImage(image_source.scaled(TAILLEH,TAILLEV,Qt::KeepAspectRatio)));
    label_source->adjustSize();

    bouton_open = new QPushButton("Ouvrir");
    connect(bouton_open,SIGNAL(clicked()), this, SLOT(open()) );
    layoutFusion->addWidget(bouton_open, 2, 0, 1, 2);


    /*
    label_fusion->setText("Cliquer pour ouvrir");
    label_fusion->setAlignment(Qt::AlignCenter);
    label_fusion->adjustSize();
*/
    image_result = image_source.copy();
    image_result.convertToFormat(QImage::Format_ARGB32,0);
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

        label_fusion->setPixmap(QPixmap::fromImage(image_fusion.scaled(TAILLEH,TAILLEV,Qt::KeepAspectRatio)));
        label_fusion->setMaximumSize(TAILLEH,TAILLEV);
        label_fusion->setMinimumSize(TAILLEH,TAILLEV);
        label_fusion->adjustSize();
        //bouton_open->hide();
        //layoutFusion->addWidget(label_fusion, 1, 0, 1, 2);
        bouton_open->setText("Ouvrir une autre image");
        image_result = image_source.copy();
        image_result.convertToFormat(QImage::Format_ARGB32,0);
        image_result = effectuer_fusion(image_fusion,image_source, image_result,type_fusion,transp_fusion,0,0);
        emit changer_image_sans_save(image_result);
    }
}


void FenetreFusion::clic_ok(){
    finir_fusion(true);
}

void FenetreFusion::clic_annuler(){
    finir_fusion(false);

}

void FenetreFusion::finir_fusion(bool b){
    this->hide();

    if(b) // faire la fusion
        emit changer_image(image_result);
    else //annuler
        emit changer_image_sans_save(image_source);

    emit changer_mode(SELECTION);
}

/* FONCTION UTILISANT LA BIBLIOTHEQUE QT
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
*/

//px, py=coordonnees du coin superieur gauche de l'image source, si les coordonnees de celui de l'image destination sont (0,0)
void FenetreFusion::fusion_basique(QImage dest, QImage src, QImage res){
    effectuer_fusion(dest,src,res, DESTINATION,1,0,0);

}

QImage FenetreFusion::effectuer_fusion(QImage dest, QImage src, QImage res, typeMode t, float transparence, int px, int py){

    QImage res1(res.width(),res.height(),QImage::Format_ARGB32);
    if ((!src.isNull())&&(!dest.isNull()))
    {


        //calculer la fin de la zone de chevauchement de deux images
        int fin_x,fin_y,i,j;
        if (px+src.width()<=dest.width())
            fin_x=px+src.width();
        else
            fin_x=dest.width();

        if (py+src.height()<=dest.height())
            fin_y=py+src.height();
        else
            fin_y=dest.height();

        //si l'user a choisi une mode de composition differente de NONE
        if (t!=NONE){

            int r, g, b;
            float as, ad;
            int v, x, y, zz;

            //conversion de l'image res en format qui supporte alpha
            //res.convertToFormat(QImage::Format_ARGB32,0);

            //les valeurs de variables pour la formule (ci-dessous) selon la mode de composition:
            switch (t)
            {
            case SOURCE: {
                    v=1;
                    x=1;
                    y=1;
                    zz=0;
                    qDebug()<<"on est rentre ici!"<<v;
                    break;
                }
            case DESTINATION:
                {
                    v=2;
                    x=1;
                    y=0;
                    zz=1;
                    break;
                }
            case SOURCE_OVER:
                {
                    v=1;
                    x=1;
                    y=1;
                    zz=1;
                    break;
                }
            case DESTINATION_OVER: {
                    v=2;
                    x=1;
                    y=1;
                    zz=1;
                    break;
                }
            case SOURCE_IN:
                {
                    v=1;
                    x=1;
                    y=0;
                    zz=0;
                    break;
                }
            case DESTINATION_IN:
                {
                    v=2;
                    x=1;
                    y=0;
                    zz=0;
                    break;
                }
            case SOURCE_OUT: {
                    v=0;
                    x=0;
                    y=1;
                    zz=0;
                    break;
                }
            case DESTINATION_OUT:
                {
                    v=0;
                    x=0;
                    y=0;
                    zz=1;
                    break;
                }
            case SOURCE_ATOP:
                {
                    v=1;
                    x=1;
                    y=0;
                    zz=1;
                    break;
                }
            case DESTINATION_ATOP:
                {
                    v=2;
                    x=1;
                    y=1;
                    zz=0;
                    break;
                }
            case CLEAR:
                {
                    v=0;
                    x=0;
                    y=0;
                    zz=0;
                    break;
                }
            case XOR:
                {
                    v=0;
                    x=0;
                    y=1;
                    zz=1;
                    break;
                }
            case NONE:
                {
                    //on ne rentre pas ici, ce cas sert a supprimer le warning
                }
            }

            //parcours de l'image resultat
            for (i=0 ; i<res.width() ; i++)
            {
                for(j=0 ; j<res.height() ; j++)
                {
                    //recuperer la couleur de pixel de l'image destination
                    QColor cd = dest.pixel(i,j);
                    //recuperer et definir la valeur de alpha (par defaut ,alpha=255 dans le constructeur QColor)
                    if (dest.hasAlphaChannel()){
                        const QRgb pix=dest.pixel(i,j);
                        cd.setAlpha(qAlpha( pix ));
                        ad=cd.alphaF();
                    }
                    //si l'image n'a pas d'alpha, alors alpha=1(i.e. 255) de partout
                    else
                        ad=1;

                    //if (cont<200){
                        //qDebug()<<"red:"<<cd.red()<<", green:"<<cd.green()<<", blue:"<<cd.blue()<<", alpha:"<<cd.alpha();
                        //cont++;
                    //}

                    QColor cs;

                    //si nous sommes en dehors de l'image source
                    if ((i<px)||(j<py)||(i>=px+src.width())||(j>=py+src.height())){
                        cs = QColor(qRgba(0,0,0,0)); //couleur transparente, ici definie comme noir
                        as=0;
                    }
                    //si nous sommes a l'interieur de l'image source
                    else{
                        //recuperer la couleur de pixel de l'image source
                        cs = src.pixel(i-px,j-py);
                        //si l'image src a le canal alpha, as=ce_canal*transparence, sinon as=transparence
                        if (src.hasAlphaChannel()){
                            //recuperer et definir la valeur de alpha (par defaut ,alpha=255 dans le constructeur QColor)
                            const QRgb pix_s=src.pixel(i-px,j-py);
                            cs.setAlpha(qAlpha( pix_s ));
                            as=cs.alphaF()*transparence;
                        }
                        //si l'image ne supporte pas alpha, alors alpha=transparence
                        else
                            as=transparence;
                    }

                    //recuperation des valeurs de variables r,g,b selon la mode
                    if (v==0){
                        r=0;
                        g=0;
                        b=0;
                    }
                    else{
                        if (v==1){
                            r=cs.red();
                            g=cs.green();
                            b=cs.blue();
                        }
                        else{
                            r=cd.red();
                            g=cd.green();
                            b=cd.blue();
                        }
                    }
                    //on applique la formule et cree ainsi une image resultat
                    res1.setPixel(i,j,QColor(r*as*ad+y*cs.red()*(1-ad)+zz*cd.red()*(1-as),
                                           g*as*ad+y*cs.green()*(1-ad)+zz*cd.green()*(1-as),
                                           b*as*ad+y*cs.blue()*(1-ad)+zz*cd.blue()*(1-as),
                                           (x*as*ad+y*as*(1-ad)+zz*ad*(1-as))*255).rgba());
                    /*qDebug()<<"r="<<r<<"as="<<as<<"ad="<<ad<<"y="<<y<<"cs red="<<cs.red()<<"zz="<<zz
                            <<"cd red="<<cd.red()<<"x="<<x<<"cs green="<<cs.green()<<"cd green="<<cd.green()
                            <<"cs blue="<<cs.blue()<<"cd blue="<<cd.blue();
                    qDebug()<<"****************************************";
                    qDebug()<<"r:"<<r*as*ad+y*cs.red()*(1-ad)+zz*cd.red()*(1-as)<<"g:"
                            <<g*as*ad+y*cs.green()*(1-ad)+zz*cd.green()*(1-as)<<"b:"
                            <<b*as*ad+y*cs.blue()*(1-ad)+zz*cd.blue()*(1-as)<<"a:"
                            <<x*as*ad+y*as*(1-ad)+zz*ad*(1-as);*/
                }
            }
        }

        //si la mode de composition=NONE, on n'applique que la transparence
        else{
            /*float tr_inv = 1-transparence; //optimisation de code

            for (i=px ; i<fin_x ; i++)
            {
                for(j=py ; j<fin_y ; j++)
                {
                    QColor c1 = src.pixel(i-px,j-py);
                    QColor c2 = dest.pixel(i,j);
                    //on modifie l'image resultat d'apres la formule de Alpha Blending
                    res.setPixel(i,j,qRgb(c1.red()*transparence + c2.red()*tr_inv,
                                          c1.green()*transparence + c2.green()*tr_inv,
                                          c1.blue()*transparence + c2.blue()*tr_inv));
                }
            }

            //copier les parties de l'image dest, qui ne se chevauchent pas avec src, dans res
            //a gauche de src
            for (i=0;i<px;i++){
                for (j=0;j<dest.height();j++){
                    QColor c = dest.pixel(i,j);
                    res.setPixel(i,j,qRgb(c.red(),c.green(),c.blue()));
                }
            }
            //a droit de src
            for (i=px+src.width();i<dest.width();i++){
                for (j=0;j<dest.height();j++){
                    QColor c = dest.pixel(i,j);
                    res.setPixel(i,j,qRgb(c.red(),c.green(),c.blue()));
                }
            }
            //en haut au milieu
            for (i=px;i<fin_x;i++){
                for (j=0;j<py;j++){
                    QColor c = dest.pixel(i,j);
                    res.setPixel(i,j,qRgb(c.red(),c.green(),c.blue()));
                }
            }
            //en bas au milieu
            for (i=px;i<fin_x;i++){
                for (j=py+src.height();j<dest.height();j++){
                    QColor c = dest.pixel(i,j);
                    res.setPixel(i,j,qRgb(c.red(),c.green(),c.blue()));
                }
            }
        }*/
    }
        //qDebug()<<"=========================================================";

        /*for (i=0;i<res.width();i++){
            for (j=0; j<res.height();j++){
                const QRgb p=res1.pixel(i,j);
                qDebug()<<"red:"<<qRed(p)<<"green:"<<qGreen(p)<<"blue:"<<qBlue(p)<<"alpha:"<<qAlpha(p);
            }
        }*/

    }
    return res1;
}




BoutonOuverture::BoutonOuverture(QWidget *parent) :
        QLabel(parent)
{

}

void BoutonOuverture::mousePressEvent(QMouseEvent *event)
{
    emit clic_bouton();
}

