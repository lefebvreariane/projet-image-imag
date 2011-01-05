#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);


    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);
    scrollArea->setAlignment(Qt::AlignCenter);
    createActions();
    createMenus();

    resize(500,400);
}



void MainWindow::createActions()
{
    exitAct = new QAction(tr("&Quitter"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    ouvrirAct = new QAction(tr("&Ouvrir"), this);
    connect(ouvrirAct, SIGNAL(triggered()), this , SLOT(open()));

    saveAct = new QAction(tr("&Enregistrer"), this);
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
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        imageLabel->setPixmap(QPixmap::fromImage(image));
        imageLabel->adjustSize();
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
        imgW->write(imageLabel->pixmap()->toImage());
    }

}
