#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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



private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *exitAct;
    QAction *ouvrirAct;
    QAction *saveAct;
    QAction *saveInAct;

    QLabel *imageLabel;
    QScrollArea *scrollArea;

};


#endif
