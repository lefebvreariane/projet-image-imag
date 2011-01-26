#ifndef FENETREFUSION_H
#define FENETREFUSION_H
#include <QtGui>
#include <controleur.h>


enum typeMode { SOURCE, DESTINATION, SOURCE_OVER, DESTINATION_OVER, SOURCE_IN, DESTINATION_IN,
                 SOURCE_OUT, DESTINATION_OUT, SOURCE_ATOP, DESTINATION_ATOP, CLEAR, XOR, NONE };


class FenetreFusion : public QFrame
{
    Q_OBJECT
public:
    explicit FenetreFusion(QWidget *parent = 0);
    void initFusion(QImage i);
    void finir_fusion(bool b);

signals:
    void changer_image(QImage);
    void changer_image_sans_save(QImage);
    void changer_mode(Mode);

public slots:
    void open();
    void clic_ok();
    void clic_annuler();
    void changement_tansparence(int);
    void changement_liste(int);
    void fusion_basique(QImage,QImage,QImage);

private :
        static const int TAILLEH = 200;
static const int TAILLEV = 200;
//QImage effectuer_fusion(QImage src, QImage dest, QImage res);

QLabel *label_source;
QLabel *label_fusion;
QLabel *label_transp_pourcentage;
QImage effectuer_fusion(QImage dest, QImage src, QImage res, typeMode t, float transparence, int px, int py);

QImage image_fusion;
QImage image_source;
QImage image_result;
QPushButton *bouton_open;

QGridLayout * layoutFusion;
QSlider *tansparence;

typeMode type_fusion;
float transp_fusion;


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
