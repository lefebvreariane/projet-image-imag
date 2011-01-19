#include <QtGui>
#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow()
{
    createActions();
    createToolBars();
    createAreas();
    createMenus();
    createStatusBar();

    createControleur();
    resize(800,600);

}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("Fichier"));
    fileToolBar->addAction(ouvrirAct);
    fileToolBar->addAction(saveAct);
}

void MainWindow::createAreas()
{

    QWidget *zoneTravail= new QWidget;
    z = new ZoneDessin;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(z->resultLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout;

    //Ajout du panneau pipette
    panneauDroite = new QFrame;
    QVBoxLayout *layoutPanneauDroite = new QVBoxLayout;
    panneauDroite->setMaximumWidth(TAILLE_PANNEAU_LATERAL);
    panneauDroite->setMinimumWidth(TAILLE_PANNEAU_LATERAL);
    panneauDroite->setMinimumHeight(250);
    fenetrePipette = new FenetrePipette;
    layoutPanneauDroite->addWidget(fenetrePipette);
    panneauDroite->setLayout(layoutPanneauDroite);
    panneauDroite->hide();

    //Ajout de la fenetre histogramme
    fenetreHistogramme = new FenetreHistogramme;

    //ajout du paneau de la fusion
    fenetreFusion = new FenetreFusion;
    fenetreFusion->hide();

    //Ajout de la fenetre redimentionnement
    fenetreRedim = new FenetreRedim;
    fenetreRedim->hide();


    layout->addWidget(scrollArea);
    layout->addWidget(panneauDroite);
    layout->addWidget(fenetreFusion);
    layout->addWidget(fenetreRedim);


    zoneTravail->setLayout(layout);
    zoneTravail->show();

    setCentralWidget(zoneTravail);

}

void MainWindow::createControleur()
{
    c = new Controleur(z);
    connect(z->resultLabel, SIGNAL(clic()), c, SLOT(clic_recu()));
    connect(c, SIGNAL(afficher_pixel(int,int,int)), fenetrePipette, SLOT(afficher_pixel(int,int,int)));
    connect(fenetrePipette, SIGNAL(afficher_panneauDroite(bool)),this,SLOT(afficher_panneauDroite(bool)));
    connect(fenetreFusion,SIGNAL(changer_image(QImage)),c->z,SLOT(changer_image(QImage)));
    connect(fenetreFusion, SIGNAL(changer_mode(Mode)), c, SLOT(changer_mode(Mode)) );
    connect(fenetreHistogramme,SIGNAL(masquer_fenetre()),this,SLOT(masquer_histogramme()));
    connect(fenetreRedim,SIGNAL(redim(int,int)),c,SLOT(redimensionner(int,int)));

    c->changer_mode(SELECTION);
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Prêt"));
}

void MainWindow::createActions()
{
    exitAct = new QAction(tr("Quitter"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    ouvrirAct = new QAction(QIcon(":/icones/open.png"), tr("Ouvrir..."), this);
    connect(ouvrirAct, SIGNAL(triggered()), this , SLOT(open()));

    saveAct = new QAction(QIcon(":/icones/save.png"),tr("Enregistrer"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveInAct = new QAction(tr("Enregistrer sous..."), this);
    connect(saveInAct, SIGNAL(triggered()), this, SLOT(saveIn()));

    pipetteAct = new QAction(tr("Pipette"), this);
    connect(pipetteAct, SIGNAL(triggered()), this, SLOT(pipette()));

    selectionAct = new QAction(tr("Selection"), this);
    connect(selectionAct, SIGNAL(triggered()), this, SLOT(selection()));

    histoAct = new QAction(tr("Histogrammes"), this);
    connect(histoAct, SIGNAL(triggered()), this, SLOT(afficher_histogramme()));

    RGB_to_greyAct = new QAction(tr("Niveaux de gris"), this);
    connect(RGB_to_greyAct, SIGNAL(triggered()), this, SLOT(RGB_to_grey()));

    flouAct = new QAction(tr("Flou"), this);
    connect(flouAct, SIGNAL(triggered()), this, SLOT(appliquer_flou()));

    fusionAct = new QAction(tr("Fusion"), this);
    connect(fusionAct, SIGNAL(triggered()), this, SLOT(fusion()));

    decoupageAct = new QAction(tr("Decoupage"), this);
    connect(decoupageAct, SIGNAL(triggered()), this, SLOT(decouper()));

    redimAct = new QAction(tr("Redimentionnement"), this);
    connect(redimAct, SIGNAL(triggered()), this, SLOT(redimentionner()));
}

void MainWindow::createMenus()
{
    /*Creation de la barre de menu Fichier*/
    fileMenu = menuBar()->addMenu("&Fichier");
    fileMenu->addAction(ouvrirAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveInAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    saveAct->setDisabled(true);
    saveInAct->setDisabled(true);

    /*Creation de la barre de menu Edition*/
    //editMenu = menuBar()->addMenu("&Edition");

    /*Creation de la barre de menu Outils*/
    toolsMenu = menuBar()->addMenu("&Outils");
    toolsMenu->addAction(selectionAct);
    toolsMenu->addAction(pipetteAct);
    toolsMenu->addAction(histoAct);
    toolsMenu->addAction(RGB_to_greyAct);
    toolsMenu->addAction(flouAct);
    toolsMenu->addAction(fusionAct);
    toolsMenu->addAction(decoupageAct);
    toolsMenu->addAction(redimAct);




}

void MainWindow::MAJ_affichage()
{
    afficher_panneauDroite(false);

    switch (c->mode) {
    case REDIM:
        fenetreRedim->show();
        break;

    default:
        fenetreRedim->hide();
        break;
    }

}

void MainWindow::verifier_fusion()
{
    if (c->mode == FUSION) // On a quitte la fusion, il faut annuler
        fenetreFusion->finir_fusion(false);
}

void MainWindow::open()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("ImageViewer"), tr("Imppossible d'ouvrir %1.").arg(fileName));
            return;
        }
        z->image = image;
        z->afficher_image();
        saveAct->setEnabled(true);
        saveInAct->setEnabled(true);
        c->reInitSelection();

        if (c->mode == FUSION)
            fusion();
        c->changer_mode(SELECTION);
        MAJ_affichage();

    }
}


void MainWindow::save()
{
    if ( fileName.isEmpty())
        saveIn();

    else
    {
        QImageWriter *imgW = new QImageWriter();
        imgW->setFileName(fileName);
        z->ecrire_image();
        imgW->write(z->image);
    }
}

void MainWindow::saveIn()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer fichier"), QDir::currentPath(),"*.jpg\n *.jpeg\n *.png\n *.bmp\n *.ppm\n *.tiff\n *.xbm\n *.xpm\n");

    QImageWriter *imgW = new QImageWriter();
    imgW->setFileName(fileName);
    z->ecrire_image();
    imgW->write(z->image);



}

void MainWindow::selection()
{
    verifier_fusion();
    c->changer_mode(SELECTION);
    qDebug()<<"selection";
    MAJ_affichage();
}

void MainWindow::pipette()
{
    verifier_fusion();
    c->changer_mode(PIPETTE);
    qDebug()<<"pipette";
    MAJ_affichage();
}


void MainWindow::redimentionner()
{
    verifier_fusion();
    c->changer_mode(REDIM);
    qDebug()<<"redim";
    fenetreRedim->initialiser(z->image);
    fenetreRedim->MAJ_valeurs_redim();
    MAJ_affichage();
}


void MainWindow::afficher_histogramme()
{
    verifier_fusion();
    c->changer_mode( HISTO);
    qDebug()<<"afficher_histogramme";
    fenetreHistogramme->histogramme->image = z->image;
    fenetreHistogramme->histogramme->repaint();
    fenetreHistogramme->show();
    MAJ_affichage();
}

void MainWindow::masquer_histogramme(){
    fenetreHistogramme->hide();
}

void MainWindow::RGB_to_grey()
{
    verifier_fusion();
    c->changer_mode(SELECTION);

    c->RGB_to_grey();
    qDebug()<<"RGB_to_grey";

    MAJ_affichage();
}

void MainWindow::appliquer_flou()
{
    verifier_fusion();
    c->changer_mode(SELECTION);

    c->appliquer_flou();
    qDebug()<<"appliquer_flou";
    MAJ_affichage();
}

void MainWindow::fusion()
{
    c->changer_mode(FUSION);
    qDebug()<<"fusion";
    fenetreFusion->fusion(z->image);
    fenetreFusion->show();

    MAJ_affichage();
}

void MainWindow::decouper()
{
    verifier_fusion();
    c->changer_mode(SELECTION);

    z->image = c->decouper();
    z->afficher_image();
    c->reInitSelection();
    qDebug() << "decoupage";
    MAJ_affichage();
}

void MainWindow::afficher_panneauDroite(bool b)
{
    if (b)
        panneauDroite->show();
    else
        panneauDroite->hide();
    ;
}


