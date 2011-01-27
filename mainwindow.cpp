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

    fenetreGradient = new FenetreGradient;
    fenetreGradient->hide();

    fenetreRehausseur= new FenetreRehausseur;
    fenetreRehausseur->hide();

    fenetreLuminosite = new FenetreLuminosite;
    fenetreLuminosite->hide();

    fenetreSeuil = new FenetreSeuil;
    fenetreSeuil->hide();

    fenetreContours = new FenetreContours;
    fenetreContours->hide();

    layout->addWidget(scrollArea);
    layout->addWidget(panneauDroite);
    layout->addWidget(fenetreFusion);
    layout->addWidget(fenetreRedim);
    layout->addWidget(fenetreFlous);
    layout->addWidget(fenetreFiltres);
    layout->addWidget(fenetreGradient);
    layout->addWidget(fenetreRehausseur);
    layout->addWidget(fenetreLuminosite);
    layout->addWidget(fenetreSeuil);
    layout->addWidget(fenetreContours);



    zoneTravail->setLayout(layout);
    zoneTravail->show();

    setCentralWidget(zoneTravail);

}

void MainWindow::createControleur()
{
    c = new Controleur(z);
    connect(c,SIGNAL(changer_message_barre(QString,int)),this,SLOT(changer_message_barre(QString,int)));
    connect(z->resultLabel, SIGNAL(clic()), c, SLOT(clic_recu()));
    connect(z,SIGNAL(enable_undo_redo()),this,SLOT(enable_undo_redo()));
    connect(z,SIGNAL(fusionner(QImage,QImage,QImage)),fenetreFusion,SLOT(fusion_basique(QImage,QImage,QImage)));
    connect(c, SIGNAL(afficher_pixel(int,int,int)), fenetrePipette, SLOT(afficher_pixel(int,int,int)));
    connect(fenetrePipette, SIGNAL(afficher_panneauDroite(bool)),this,SLOT(afficher_panneauDroite(bool)));
    connect(fenetreFusion,SIGNAL(changer_image(QImage)),c->z,SLOT(changer_image(QImage)));
    connect(fenetreFusion,SIGNAL(changer_image_sans_save(QImage)),c->z,SLOT(changer_image_sans_save(QImage)));
    connect(fenetreFusion, SIGNAL(changer_mode(Mode)), c, SLOT(changer_mode(Mode)) );
    connect(fenetreRedim,SIGNAL(redim(int,int,int)),c,SLOT(redimensionner(int,int,int)));
    connect(fenetreFlous,SIGNAL(appliquer_flou(int,TypeConvo)),c,SLOT(appliquer_flou(int,TypeConvo)));
    connect(fenetreFlous,SIGNAL(appliquer_mediane(int)),c,SLOT(appliquer_median(int)));
    connect(fenetreFiltres,SIGNAL(appliquer_filtre_perso(MatConvo*)),c,SLOT(appliquer_filtre(MatConvo*)));
    connect(fenetreGradient,SIGNAL(appliquer_gradient_x(TypeConvo)),c,SLOT(appliquer_gradient_x(TypeConvo)));
    connect(fenetreGradient,SIGNAL(appliquer_gradient_y(TypeConvo)),c,SLOT(appliquer_gradient_y(TypeConvo)));
    connect(fenetreGradient,SIGNAL(appliquer_gradient_moins_x(TypeConvo)),c,SLOT(appliquer_gradient_moins_x(TypeConvo)));
    connect(fenetreGradient,SIGNAL(appliquer_gradient_moins_y(TypeConvo)),c,SLOT(appliquer_gradient_moins_y(TypeConvo)));
    connect(fenetreGradient,SIGNAL(norme_gradient(TypeConvo)),c,SLOT(norme_gradient(TypeConvo)));
    connect(fenetreGradient,SIGNAL(norme_4gradients(TypeConvo)),c,SLOT(norme_4gradients(TypeConvo)));
    connect(fenetreGradient,SIGNAL(appliquer_laplacien(int,int)),c,SLOT(appliquer_laplacien(int,int)));
    connect(fenetreRehausseur,SIGNAL(appliquer_rehausseur_laplacien(int,int)),c,SLOT(appliquer_rehausseur_laplacien(int,int)));
    connect(fenetreLuminosite,SIGNAL(appliquer_luminosite_contraste(float,float)),c,SLOT(luminosite_contraste(float,float)));
    connect(fenetreSeuil,SIGNAL(appliquer_seuil(int)),c,SLOT(seuillage(int)));
    connect(fenetreContours,SIGNAL(detection_contours(int,int,TypeConvo)),c,SLOT(chainage_contours(int,int,TypeConvo)));

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
    keyDecoup= new QShortcut(QKeySequence::Delete,this);
    keyOpen = new QShortcut(QKeySequence::Open,this);
    keySave = new QShortcut(QKeySequence::Save,this);
    keyQuit = new QShortcut(QKeySequence::Quit,this);
    keyUndo = new QShortcut(QKeySequence::Undo,this);
    keyRedo = new QShortcut(QKeySequence::Redo,this);
    keySelec = new QShortcut((tr("Ctrl+S")),this);
    keyPipette = new QShortcut((tr("Ctrl+P")),this);
    keyHisto = new QShortcut((tr("Ctrl+H")),this);
    keyRedim = new QShortcut((tr("Ctrl+R")),this);


    exitAct = new QAction(QIcon(":/icones/exit.png"),tr("Quitter"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(keyQuit, SIGNAL(activated()),this, SLOT(close()));


    ouvrirAct = new QAction(QIcon(":/icones/open.png"), tr("Ouvrir..."), this);
    connect(ouvrirAct, SIGNAL(triggered()), this , SLOT(open()));
    connect(keyOpen, SIGNAL(activated()),this, SLOT(open()));


    saveAct = new QAction(QIcon(":/icones/save.png"),tr("Enregistrer"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    connect(keySave, SIGNAL(activated()),this, SLOT(save()));


    saveInAct = new QAction(QIcon(":/icones/saveas.png"),tr("Enregistrer sous..."), this);
    connect(saveInAct, SIGNAL(triggered()), this, SLOT(saveIn()));

    undoAct = new QAction(QIcon(":/icones/undo.png"),tr("Annuler"), this);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));
    connect(keyUndo, SIGNAL(activated()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/icones/redo.png"),tr("Refaire"), this);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));
    connect(keyRedo, SIGNAL(activated()), this, SLOT(redo()));


    pipetteAct = new QAction(QIcon(":/icones/pipette.png"),tr("Pipette"), this);
    connect(pipetteAct, SIGNAL(triggered()), this, SLOT(pipette()));
    connect(keyPipette, SIGNAL(activated()),this, SLOT(pipette()));
    pipetteAct->setCheckable(true);


    selectionAct = new QAction(QIcon(":/icones/selection.png"),tr("Selection"), this);
    connect(selectionAct, SIGNAL(triggered()), this, SLOT(selection()));
    connect(keySelec, SIGNAL(activated()),this, SLOT(selection()));
    selectionAct->setCheckable(true);


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

    decoupageIntelligentAct = new QAction(tr("Decoupage intelligent"), this);
    connect(decoupageIntelligentAct, SIGNAL(triggered()), this, SLOT(decoupage_intel()));

    baguetteAct = new QAction(QIcon(":/icones/cutmagique.png"),tr("Gomme magique"), this);
    connect(baguetteAct, SIGNAL(triggered()), this, SLOT(baguette_magique()));
    baguetteAct->setCheckable(true);

    redimAct = new QAction(tr("Redimentionnement"), this);
    connect(redimAct, SIGNAL(triggered()), this, SLOT(redimentionner()));
    connect(keyRedim, SIGNAL(activated()),this, SLOT(redimentionner()));

    gradientAct = new QAction(tr("Gradient"), this);
    connect(gradientAct, SIGNAL(triggered()), this, SLOT(gradient()));

    rehaussAct = new QAction(tr("Rehausseur Laplacien"), this);
    connect(rehaussAct, SIGNAL(triggered()), this, SLOT(rehausseur()));

    lumAct = new QAction(tr("Luminosité / Contraste"), this);
    connect(lumAct, SIGNAL(triggered()), this, SLOT(luminosite()))
            ;
    seuilAct = new QAction(tr("Seuil"), this);
    connect(seuilAct, SIGNAL(triggered()), this, SLOT(seuil()));

    contoursAct = new QAction(tr("Detection de contours"), this);
    connect(contoursAct, SIGNAL(triggered()), this, SLOT(detection_coutours()));

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

    /*Creation de la barre de menu Edition*/
    editMenu = menuBar()->addMenu("&Edition");
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    /*Creation de la barre de menu Outils*/
    toolsMenu = menuBar()->addMenu("&Outils");
    toolsMenu->addAction(selectionAct);
    toolsMenu->addAction(pipetteAct);
    toolsMenu->addAction(baguetteAct);
    toolsMenu->addAction(decoupageAct);
    toolsMenu->addAction(decoupageIntelligentAct);
    toolsMenu->addAction(histoAct);
    toolsMenu->addAction(RGB_to_greyAct);
    toolsMenu->addAction(inversAct);
    toolsMenu->addAction(fusionAct);
    toolsMenu->addAction(redimAct);
    toolsMenu->addAction(lumAct);
    toolsMenu->addAction(seuilAct);

    /*Creation de la barre de menu Filtres*/
    filtreMenu = menuBar()->addMenu("&Filtres");
    filtreMenu->addAction(flouAct);
    filtreMenu->addAction(medianAct);
    filtreMenu->addAction(gradientAct);
    filtreMenu->addAction(rehaussAct);
    filtreMenu->addAction(contoursAct);
    fileMenu->addSeparator();
    filtreMenu->addAction(filtreAct);


    activer_menus(false);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("Fichier"));
    fileToolBar->addAction(ouvrirAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(undoAct);
    fileToolBar->addAction(redoAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(selectionAct);
    fileToolBar->addAction(pipetteAct);
    //fileToolBar->addAction(baguetteAct);


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
        decoupageIntelligentAct->setDisabled(true);
        baguetteAct->setDisabled(true);
        redimAct->setDisabled(true);
        flouAct->setDisabled(true);
        medianAct->setDisabled(true);
        filtreAct->setDisabled(true);
        gradientAct->setDisabled(true);
        rehaussAct->setDisabled(true);
        lumAct->setDisabled(true);
        seuilAct->setDisabled(true);
        contoursAct->setDisabled(true);

        keyDecoup->setEnabled(false);
        keyOpen->setEnabled(false);
        keySave->setEnabled(false);
        keyQuit->setEnabled(false);
        keySelec->setEnabled(false);
        keyPipette->setEnabled(false);
        keyHisto->setEnabled(false);
        keyRedim->setEnabled(false);


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
        decoupageIntelligentAct->setDisabled(false);
        baguetteAct->setDisabled(false);
        redimAct->setDisabled(false);
        flouAct->setDisabled(false);
        medianAct->setDisabled(false);
        filtreAct->setDisabled(false);
        gradientAct->setDisabled(false);
        rehaussAct->setDisabled(false);
        lumAct->setDisabled(false);
        seuilAct->setDisabled(false);
        contoursAct->setDisabled(false);

        keyDecoup->setEnabled(true);
        keyOpen->setEnabled(true);
        keySave->setEnabled(true);
        keyQuit->setEnabled(true);
        keySelec->setEnabled(true);
        keyPipette->setEnabled(true);
        keyHisto->setEnabled(true);
        keyRedim->setEnabled(true);
    }

    undoAct->setDisabled(true);
    redoAct->setDisabled(true);
    keyUndo->setEnabled(false);
    keyRedo->setEnabled(false);
}

void MainWindow::MAJ_affichage()
{
    afficher_panneauDroite(false);

    if (c->mode == PIPETTE)
        pipetteAct->setChecked(true);
    else
        pipetteAct->setChecked(false);

    if (c->mode == SELECTION)
        selectionAct->setChecked(true);
    else
        selectionAct->setChecked(false);

    if (c->mode == BAGUETTE_MAGIQUE)
        baguetteAct->setChecked(true);
    else
        baguetteAct->setChecked(false);


    if (c->mode == REDIM)
        fenetreRedim->show();
    else
        fenetreRedim->hide();

    if (c->mode == HISTO)
        fenetreHistogramme->show();
    else
        fenetreHistogramme->hide();

    if(c->mode == FLOU)
        fenetreFlous->show();
    else
        fenetreFlous->hide();

    if(c->mode == FILTRE)
        fenetreFiltres->show();
    else
        fenetreFiltres->hide();

    if(c->mode == GRADIENT)
        fenetreGradient->show();
    else
        fenetreGradient->hide();

    if(c->mode == REHAUSSEUR)
        fenetreRehausseur->show();
    else
        fenetreRehausseur->hide();

    if(c->mode == LUMINOSITE)
        fenetreLuminosite->show();
    else
        fenetreLuminosite->hide();

    if(c->mode == SEUIL)
        fenetreSeuil->show();
    else
        fenetreSeuil->hide();

    if(c->mode == CONTOURS)
        fenetreContours->show();
    else
        fenetreContours->hide();


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
        z->changer_image_sans_save(image);
        z->historique->init_historique(z->image);
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
    MAJ_affichage();
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

void MainWindow::decoupage_intel()
{
    verifier_fusion();
    c->changer_mode(DECOUPAGE_INTEL);
    //c->decoupage_intelligent_contours();
    //c->reInitSelection();
    statusBar()->showMessage("Cliquer pour decouper intelligement ");
    MAJ_affichage();
}

void MainWindow::baguette_magique()
{
    verifier_fusion();
    c->changer_mode(BAGUETTE_MAGIQUE);
    statusBar()->showMessage("Cliquer pour utiliser la baguette magique ");
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

void  MainWindow::gradient(){
    verifier_fusion();
    c->changer_mode(GRADIENT);
    MAJ_affichage();

}

void  MainWindow::rehausseur(){
    verifier_fusion();
    c->changer_mode(REHAUSSEUR);
    MAJ_affichage();

}

void  MainWindow::undo(){
    verifier_fusion();
    z->historique->undo();
    enable_undo_redo();

}

void  MainWindow::redo(){

    verifier_fusion();
    z->historique->redo();
    enable_undo_redo();

}

void  MainWindow::enable_undo_redo(){
    if (z->historique->can_undo()){
        undoAct->setEnabled(true);
        keyUndo->setEnabled(true);
    }
    else{
        undoAct->setEnabled(false);
        keyUndo->setEnabled(false);
    }

    if (z->historique->can_redo()){
        redoAct->setEnabled(true);
        keyRedo->setEnabled(true);
    }
    else{
        redoAct->setEnabled(false);
        keyRedo->setEnabled(false);
    }
}


void MainWindow::luminosite(){
    verifier_fusion();
    c->changer_mode(LUMINOSITE);
    fenetreLuminosite->init_lum_cont();
    MAJ_affichage();
}

void MainWindow::seuil(){
    verifier_fusion();
    c->changer_mode(SEUIL);
    fenetreSeuil->init_seuil();
    MAJ_affichage();
}

void MainWindow::detection_coutours(){
    verifier_fusion();
    c->changer_mode(CONTOURS);
    MAJ_affichage();
}

