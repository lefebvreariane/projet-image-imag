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
    //connect(z->resultLabel,SIGNAL(clicked()),this,clic_recu());


    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(z->resultLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout;
    QFrame *panneauDroite = new QFrame;

    panneauDroite->setMaximumWidth(TAILLE_PANNEAU_LATERAL);
    panneauDroite->setMinimumWidth(TAILLE_PANNEAU_LATERAL);

    layout->addWidget(scrollArea);
    layout->addWidget(panneauDroite);

    zoneTravail->setLayout(layout);
    zoneTravail->show();

    setCentralWidget(zoneTravail);

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
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Fichier"));

    fileMenu->addAction(ouvrirAct);
    fileMenu->addAction(saveAct);
    saveAct->setDisabled(true);
    fileMenu->addAction(saveInAct);
    saveInAct->setDisabled(true);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

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

void MainWindow::clic_recu()
{
    qDebug()<<"click recu";
}
