#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QPainter>
#include <QWidget>
#include <labelimage.h>


QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QToolButton;
QT_END_NAMESPACE

class ZoneDessin : public QWidget
{
    Q_OBJECT
public:

    explicit ZoneDessin(QWidget *parent = 0);
    QImage image;
    QImage image_affichages;
    QPainter *painter;
    LabelImage *resultLabel;

    void init_affichage();
    void ecrire_image();
    void afficher_image();

signals:
    void fusionner(QImage,QImage,QImage);

public slots:
    void changer_image(QImage);

};

#endif // ZONEDESSIN_H
