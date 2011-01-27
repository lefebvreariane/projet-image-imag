#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ZoneDessin.h>
#include <controleur.h>
#include <fenetrepipette.h>
#include <fenetrehistogramme.h>
#include <fenetrefusion.h>
#include <fenetreredim.h>
#include <fenetreflous.h>
#include <fenetrefiltres.h>
#include "fenetregradient.h"
#include "fenetreluminosite.h"
#include "fenetreseuil.h"
#include "fenetrerehausseur.h"
#include "fenetrecontours.h"
#include "noyaupascal.h"
#include "historique.h"




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
    void decoupage_intel();
    void baguette_magique();
    void afficher_panneauDroite(bool);
    void redimentionner();
    void median();
    void filtre_perso();
    void inverser_couleurs();
    void changer_message_barre(QString,int);
    void gradient();
    void undo();
    void redo();
    void enable_undo_redo();
    void rehausseur();
    void luminosite();
    void seuil();
    void detection_coutours();



private:
    void createAreas();
    void createActions();
    void createControleur();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void MAJ_affichage();
    void verifier_fusion();
    void activer_menus(bool b);
    static const int TAILLE_PANNEAU_LATERAL = 100;
    QString fileName;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *filtreMenu;
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
    QAction *inversAct;
    QAction *flouAct;
    QAction *fusionAct;
    QAction *decoupageAct;
    QAction *decoupageIntelligentAct;
    QAction *redimAct;
    QAction *filtreAct;
    QAction *medianAct;
    QAction *gradientAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *rehaussAct;
    QAction *lumAct;
    QAction *seuilAct;
    QAction *contoursAct;
    QAction *baguetteAct;





    QShortcut *keyDecoup;
    QShortcut *keyOpen;
    QShortcut *keySave;
    QShortcut *keyQuit;
    QShortcut *keyUndo;
    QShortcut *keyRedo;
    QShortcut *keySelec;
    QShortcut *keyPipette;
    QShortcut *keyHisto;
    QShortcut *keyRedim;



    ZoneDessin *z;
    QScrollArea *scrollArea;
    QToolBar *fileToolBar;
    FenetrePipette *fenetrePipette;
    FenetreHistogramme *fenetreHistogramme;
    FenetreFusion *fenetreFusion;
    FenetreRedim *fenetreRedim;
    FenetreFlous *fenetreFlous;
    FenetreFiltres *fenetreFiltres;
    FenetreGradient *fenetreGradient;
    FenetreRehausseur *fenetreRehausseur;
    FenetreLuminosite *fenetreLuminosite;
    FenetreSeuil *fenetreSeuil;
    FenetreContours *fenetreContours;

};




#endif
