#include <QtGui>
#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow()
{
    createActions();
    createToolBars();
    createAreas();
    createControleur();
    createMenus();
    createStatusBar();
    resize(500,400);

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
    QFrame *panneauDroite = new QFrame;
    QVBoxLayout *layoutPanneauDroite = new QVBoxLayout;

    panneauDroite->setMaximumWidth(TAILLE_PANNEAU_LATERAL);
    panneauDroite->setMinimumWidth(TAILLE_PANNEAU_LATERAL);
    fenetrePipette = new FenetrePipette;
    layoutPanneauDroite->addWidget(fenetrePipette);
    panneauDroite->setLayout(layoutPanneauDroite);

    layout->addWidget(scrollArea);
    layout->addWidget(panneauDroite);
    zoneTravail->setLayout(layout);
    zoneTravail->show();

    setCentralWidget(zoneTravail);

}

void MainWindow::createControleur()
{
    c = new Controleur(z);
    connect(z->resultLabel, SIGNAL(clic()), c, SLOT(clic_recu()));
    connect(c, SIGNAL(afficher_pixel(int,int,int)), fenetrePipette, SLOT(afficher_pixel(int,int,int)));

    c->mode = AUCUN;
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Prêt"));
}

void MainWindow::createActions()
{
    exitAct = new QAction(tr("&Quitter"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    ouvrirAct = new QAction(QIcon(":/icones/open.png"), tr("&Ouvrir..."), this);
    connect(ouvrirAct, SIGNAL(triggered()), this , SLOT(open()));

    saveAct = new QAction(QIcon(":/icones/save.png"),tr("&Enregistrer"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveInAct = new QAction(tr("&Enregistrer sous..."), this);
    connect(saveInAct, SIGNAL(triggered()), this, SLOT(saveIn()));

    pipetteAct = new QAction(tr("&Pipette"), this);
    connect(pipetteAct, SIGNAL(triggered()), this, SLOT(pipette()));

    histoAct = new QAction(tr("&Histogrammes"), this);
    connect(histoAct, SIGNAL(triggered()), this, SLOT(afficher_histogramme()));

    RGB_to_greyAct = new QAction(tr("&Niveaux de gris"), this);
    connect(RGB_to_greyAct, SIGNAL(triggered()), this, SLOT(RGB_to_grey()));

    flouAct = new QAction(tr("&Flou"), this);
    connect(flouAct, SIGNAL(triggered()), this, SLOT(appliquer_flou()));

    fusionAct = new QAction(tr("&Fusion"), this);
    connect(fusionAct, SIGNAL(triggered()), this, SLOT(fusion()));

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
    toolsMenu->addAction(pipetteAct);
    toolsMenu->addAction(histoAct);
    toolsMenu->addAction(RGB_to_greyAct);
    toolsMenu->addAction(flouAct);
    toolsMenu->addAction(fusionAct);




}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Ouvrir un fichier"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("ImageViewer"),
                                     tr("Imppossible d'ouvrir %1.").arg(fileName));
            return;
        }
        z->image = image;
        z->afficher_image();
        saveAct->setEnabled(true);
        saveInAct->setEnabled(true);

    }
}


void MainWindow::save()
{

}

void MainWindow::saveIn()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer fichier"), QDir::currentPath(),
                                                    "*.jpeg\n *.jpg\n *.png\n *.bmp\n *.ppm\n *.tiff\n *.xbm\n *.xpm\n");
    QImageWriter *imgW = new QImageWriter();


    if (!fileName.isEmpty()){
        imgW->setFileName(fileName);
        z->ecrire_image();
        imgW->write(z->image);
    }
}

void MainWindow::pipette()
{
    c->mode = PIPETTE;
    qDebug()<<"pipette";
}

void MainWindow::afficher_histogramme()
{
    c->mode = HISTO;
    qDebug()<<"afficher_histogramme";
}

void MainWindow::RGB_to_grey()
{
    c->mode = GREY;
    qDebug()<<"RGB_to_grey";
}

void MainWindow::appliquer_flou()
{
    c->mode = FLOU;
    qDebug()<<"appliquer_flou";
}

void MainWindow::fusion()
{
    c->mode = FUSION;
    qDebug()<<"fusion";
}


