#include "mainwindow.h"

MainWindow::MainWindow()
{
    createActions();
    createToolBars();
    createAreas();
    createMenus();
    createStatusBar();

    createControleur();
    resize(800,600);
    //this->showMaximized();

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

    //Ajout des fenetres de filtres
    fenetreFlous = new FenetreFlous;
    fenetreFlous->hide();

    fenetreFiltres = new FenetreFiltres;
    fenetreFiltres->hide();


    layout->addWidget(scrollArea);
    layout->addWidget(panneauDroite);
    layout->addWidget(fenetreFusion);
    layout->addWidget(fenetreRedim);
    layout->addWidget(fenetreFlous);
    layout->addWidget(fenetreFiltres);


    zoneTravail->setLayout(layout);
    zoneTravail->show();

    setCentralWidget(zoneTravail);

}

void MainWindow::createControleur()
{
    c = new Controleur(z);
    connect(c,SIGNAL(changer_message_barre(QString,int)),this,SLOT(changer_message_barre(QString,int)));
    connect(z->resultLabel, SIGNAL(clic()), c, SLOT(clic_recu()));
    connect(z,SIGNAL(fusionner(QImage,QImage,QImage)),fenetreFusion,SLOT(fusion_basique(QImage,QImage,QImage)));
    connect(c, SIGNAL(afficher_pixel(int,int,int)), fenetrePipette, SLOT(afficher_pixel(int,int,int)));
    connect(fenetrePipette, SIGNAL(afficher_panneauDroite(bool)),this,SLOT(afficher_panneauDroite(bool)));
    connect(fenetreFusion,SIGNAL(changer_image(QImage)),c->z,SLOT(changer_image(QImage)));
    connect(fenetreFusion, SIGNAL(changer_mode(Mode)), c, SLOT(changer_mode(Mode)) );
    connect(fenetreHistogramme,SIGNAL(masquer_fenetre()),this,SLOT(masquer_histogramme()));
    connect(fenetreRedim,SIGNAL(redim(int,int)),c,SLOT(redimensionner(int,int)));
    connect(fenetreFlous,SIGNAL(appliquer_flou(int,TypeConvo)),c,SLOT(appliquer_flou(int,TypeConvo)));
    connect(fenetreFlous,SIGNAL(appliquer_mediane(int)),c,SLOT(appliquer_median(int)));
    connect(fenetreFiltres,SIGNAL(appliquer_filtre_perso(MatConvo*)),c,SLOT(appliquer_flou(MatConvo*)));

    c->changer_mode(SELECTION);
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Prêt"));
}
void MainWindow::changer_message_barre(QString message,int t){
    if (t==0){
        statusBar()->showMessage(message);
    }
    else
        statusBar()->showMessage(message,t);

}

void MainWindow::createActions()
{
    QShortcut *keyDecoup= new QShortcut(QKeySequence::Delete,this);
    QShortcut *keyOpen = new QShortcut(QKeySequence::Open,this);
    QShortcut *keySave = new QShortcut(QKeySequence::Save,this);
    QShortcut *keyQuit = new QShortcut(QKeySequence::Quit,this);
    QShortcut *keySelec = new QShortcut((tr("Ctrl+S")),this);
    QShortcut *keyPipette = new QShortcut((tr("Ctrl+P")),this);
    QShortcut *keyHisto = new QShortcut((tr("Ctrl+H")),this);
    QShortcut *keyRedim = new QShortcut((tr("Ctrl+R")),this);

    exitAct = new QAction(tr("Quitter"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(keyQuit, SIGNAL(activated()),this, SLOT(close()));


    ouvrirAct = new QAction(QIcon(":/icones/open.png"), tr("Ouvrir..."), this);
    connect(ouvrirAct, SIGNAL(triggered()), this , SLOT(open()));
    connect(keyOpen, SIGNAL(activated()),this, SLOT(open()));


    saveAct = new QAction(QIcon(":/icones/save.png"),tr("Enregistrer"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    connect(keySave, SIGNAL(activated()),this, SLOT(save()));


    saveInAct = new QAction(tr("Enregistrer sous..."), this);
    connect(saveInAct, SIGNAL(triggered()), this, SLOT(saveIn()));

    pipetteAct = new QAction(tr("Pipette"), this);
    connect(pipetteAct, SIGNAL(triggered()), this, SLOT(pipette()));
    connect(keyPipette, SIGNAL(activated()),this, SLOT(pipette()));


    selectionAct = new QAction(tr("Selection"), this);
    connect(selectionAct, SIGNAL(triggered()), this, SLOT(selection()));
    connect(keySelec, SIGNAL(activated()),this, SLOT(selection()));


    histoAct = new QAction(tr("Histogrammes"), this);
    connect(histoAct, SIGNAL(triggered()), this, SLOT(afficher_histogramme()));
    connect(keyHisto, SIGNAL(activated()),this, SLOT(afficher_histogramme()));


    RGB_to_greyAct = new QAction(tr("Niveaux de gris"), this);
    connect(RGB_to_greyAct, SIGNAL(triggered()), this, SLOT(RGB_to_grey()));

    inversAct = new QAction(tr("Inverser les couleurs"), this);
    connect(inversAct, SIGNAL(triggered()), this, SLOT(inverser_couleurs()));

    flouAct = new QAction(tr("Flou"), this);
    connect(flouAct, SIGNAL(triggered()), this, SLOT(appliquer_flou()));

    medianAct = new QAction(tr("Reduction de bruit"), this);
    connect(medianAct, SIGNAL(triggered()), this, SLOT(median()));

    filtreAct = new QAction(tr("Filtre personnalisé"), this);
    connect(filtreAct, SIGNAL(triggered()), this, SLOT(filtre_perso()));

    fusionAct = new QAction(tr("Fusion"), this);
    connect(fusionAct, SIGNAL(triggered()), this, SLOT(fusion()));

    decoupageAct = new QAction(tr("Decoupage"), this);
    connect(decoupageAct, SIGNAL(triggered()), this, SLOT(decouper()));
    connect(keyDecoup, SIGNAL(activated()),this, SLOT(decouper()));

    redimAct = new QAction(tr("Redimentionnement"), this);
    connect(redimAct, SIGNAL(triggered()), this, SLOT(redimentionner()));
    connect(keyRedim, SIGNAL(activated()),this, SLOT(redimentionner()));


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

    /*Creation de la barre de menu Outils*/
    toolsMenu = menuBar()->addMenu("&Outils");
    toolsMenu->addAction(selectionAct);
    toolsMenu->addAction(pipetteAct);
    toolsMenu->addAction(histoAct);
    toolsMenu->addAction(RGB_to_greyAct);
    toolsMenu->addAction(inversAct);
    toolsMenu->addAction(fusionAct);
    toolsMenu->addAction(decoupageAct);
    toolsMenu->addAction(redimAct);

    /*Creation de la barre de menu Filtres*/
    filtreMenu = menuBar()->addMenu("&Filtres");
    filtreMenu->addAction(flouAct);
    filtreMenu->addAction(medianAct);
    filtreMenu->addAction(filtreAct);

    activer_menus(false);
}

void MainWindow::activer_menus(bool b)
{
    if (!b){

        saveAct->setDisabled(true);
        saveInAct->setDisabled(true);
        selectionAct->setDisabled(true);
        pipetteAct->setDisabled(true);
        histoAct->setDisabled(true);
        RGB_to_greyAct->setDisabled(true);
        inversAct->setDisabled(true);
        fusionAct->setDisabled(true);
        decoupageAct->setDisabled(true);
        redimAct->setDisabled(true);
        flouAct->setDisabled(true);
        medianAct->setDisabled(true);
        filtreAct->setDisabled(true);
    }
    else
    {
        saveAct->setDisabled(false);
        saveInAct->setDisabled(false);
        selectionAct->setDisabled(false);
        pipetteAct->setDisabled(false);
        histoAct->setDisabled(false);
        RGB_to_greyAct->setDisabled(false);
        inversAct->setDisabled(false);
        fusionAct->setDisabled(false);
        decoupageAct->setDisabled(false);
        redimAct->setDisabled(false);
        flouAct->setDisabled(false);
        medianAct->setDisabled(false);
        filtreAct->setDisabled(false);
    }
}

void MainWindow::MAJ_affichage()
{
    afficher_panneauDroite(false);

    if (c->mode == REDIM)
        fenetreRedim->show();
    else
        fenetreRedim->hide();

    if(c->mode == FLOU)
        fenetreFlous->show();
    else
        fenetreFlous->hide();

    if(c->mode == FILTRE)
        fenetreFiltres->show();
    else
        fenetreFiltres->hide();


    z->init_affichage();
    z->afficher_image();

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
        z->changer_image(image);
        activer_menus(true);
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
    MAJ_affichage();
    statusBar()->showMessage("Selectionner une zone");
}

void MainWindow::pipette()
{
    verifier_fusion();
    c->changer_mode(PIPETTE);
    MAJ_affichage();
    statusBar()->showMessage("Cliquer pour afficher un pixel");
}


void MainWindow::redimentionner()
{
    verifier_fusion();
    c->changer_mode(REDIM);
    fenetreRedim->initialiser(z->image);
    fenetreRedim->MAJ_valeurs_redim();
    MAJ_affichage();
    statusBar()->showMessage("Redimentionnement");
}


void MainWindow::afficher_histogramme()
{
    verifier_fusion();
    c->changer_mode( HISTO);
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
    MAJ_affichage();
    statusBar()->showMessage("L'image est en niveaux de gris");
}

void MainWindow::inverser_couleurs()
{
    verifier_fusion();
    c->changer_mode(SELECTION);
    c->inverser_couleurs();
    MAJ_affichage();
}


void MainWindow::fusion()
{
    c->changer_mode(FUSION);
    fenetreFusion->initFusion(z->image);
    fenetreFusion->show();
    MAJ_affichage();
    statusBar()->showMessage("Ouvrir un fichier pour le fusionner");
}

void MainWindow::decouper()
{
    verifier_fusion();
    c->changer_mode(SELECTION);

    z->changer_image(c->decouper());
    c->reInitSelection();
    statusBar()->showMessage("L'image a été découpée");
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

void MainWindow::appliquer_flou()
{
    verifier_fusion();
    c->changer_mode(FLOU);

    //statusBar()->showMessage("L'image a été floutée");

    fenetreFlous->Radio_lineaire->setChecked(true);
    fenetreFlous->clic_radioBouton();
    MAJ_affichage();
}

void  MainWindow::median(){
    verifier_fusion();
    c->changer_mode(FLOU);
    fenetreFlous->Radio_median->setChecked(true);
    fenetreFlous->clic_radioBouton();


    MAJ_affichage();

}

void  MainWindow::filtre_perso(){
    verifier_fusion();
    c->changer_mode(FILTRE);
    MAJ_affichage();

}

