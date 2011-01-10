#ifndef FENETREPIPETTE_H
#define FENETREPIPETTE_H
#include <QtGui>
#include <QWidget>

class FenetrePipette : public QWidget
{
    Q_OBJECT
public:
    explicit FenetrePipette(QWidget *parent = 0);
    QLabel *labelCouleur;
    QLabel *labelR;
    QLabel *labelG;
    QLabel *labelB;
    QLabel *labelY;
    QLabel *labelU;
    QLabel *labelV;




signals:
    void afficher_panneauDroite(bool);

public slots:

    void afficher_pixel(int r, int g, int b);

private:
QImage apercu;

};

#endif // FENETREPIPETTE_H
