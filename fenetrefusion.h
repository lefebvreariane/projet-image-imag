#ifndef FENETREFUSION_H
#define FENETREFUSION_H
#include <QtGui>
#include <QWidget>
#include <controleur.h>

class FenetreFusion : public QFrame
{
    Q_OBJECT
public:
    explicit FenetreFusion(QWidget *parent = 0);
    void fusion(QImage i);
    void finir_fusion(bool b);

signals:
    void changer_image(QImage);
    void changer_mode(Mode);

public slots:
    void open();
    void clic_ok();
    void clic_annuler();
private :
        static const int TAILLEH = 200;
static const int TAILLEV = 200;
QImage effectuer_fusion(QImage src, QImage dest, QImage res);
QLabel *label_source;
QLabel *label_fusion;
QLabel *label_result;

QImage image_fusion;
QImage image_source;
QImage image_result;


QGridLayout * layoutFusion;


};

class BoutonOuverture : public QLabel
{
    Q_OBJECT
public:
    explicit BoutonOuverture(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);

signals:
    void clic_bouton();
public slots:
private :
    };

#endif // FENETREFUSION_H
