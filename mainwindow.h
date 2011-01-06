#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ZoneDessin.h>


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


private slots:
    void open();
    void save();
    void saveIn();
    void clic_recu();



private:
    void createAreas();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    static const int TAILLE_PANNEAU_LATERAL = 100;
    QMenu *fileMenu;
    QAction *exitAct;
    QAction *ouvrirAct;
    QAction *saveAct;
    QAction *saveInAct;
    ZoneDessin *z;

    QScrollArea *scrollArea;
    QToolBar *fileToolBar;

};


#endif
