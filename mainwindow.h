#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ZoneDessin.h>
#include <controleur.h>
#include <fenetrepipette.h>
#include <fenetrehistogramme.h>
#include <fenetrefusion.h>
#include <fenetreredim.h>

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
    void selection();
    void pipette();
    void afficher_histogramme();
    void RGB_to_grey();
    void appliquer_flou();
    void fusion();
    void decouper();
    void afficher_panneauDroite(bool);
    void masquer_histogramme();
    void redimentionner();







private:
    void createAreas();
    void createActions();
    void createControleur();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void MAJ_affichage();
    void verifier_fusion();





    static const int TAILLE_PANNEAU_LATERAL = 100;
    QString fileName;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;

    QFrame *panneauDroite;
    QFrame *panneauFusion;

    QAction *exitAct;
    QAction *ouvrirAct;
    QAction *saveAct;
    QAction *saveInAct;

    QAction *selectionAct;
    QAction *pipetteAct;
    QAction *histoAct;
    QAction *RGB_to_greyAct;
    QAction *flouAct;
    QAction *fusionAct;
    QAction *decoupageAct;
    QAction *redimAct;

    ZoneDessin *z;
    QScrollArea *scrollArea;
    QToolBar *fileToolBar;
    FenetrePipette *fenetrePipette;
    FenetreHistogramme *fenetreHistogramme;
    FenetreFusion *fenetreFusion;
    FenetreRedim *fenetreRedim;

};




#endif
