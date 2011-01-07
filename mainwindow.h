#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ZoneDessin.h>
#include <controleur.h>
#include <fenetrepipette.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QScrollArea;
class QScrollBar;
class QAction;
class QMenu;
class QPlainTextEdit;
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    Controleur *c;



private slots:
    void open();
    void save();
    void saveIn();
    void pipette();
    void afficher_histogramme();
    void RGB_to_grey();
    void appliquer_flou();
    void fusion();





private:
    void createAreas();
    void createActions();
    void createControleur();
    void createMenus();
    void createStatusBar();
    void createToolBars();


    static const int TAILLE_PANNEAU_LATERAL = 100;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;

    QAction *exitAct;
    QAction *ouvrirAct;
    QAction *saveAct;
    QAction *saveInAct;
    QAction *pipetteAct;
    QAction *histoAct;
    QAction *RGB_to_greyAct;
    QAction *flouAct;
    QAction *fusionAct;

    ZoneDessin *z;
    QScrollArea *scrollArea;
    QToolBar *fileToolBar;
    FenetrePipette *fenetrePipette;

};




#endif
