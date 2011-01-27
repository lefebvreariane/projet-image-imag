#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QtGui>
#include <QPainter>
#include <QWidget>
#include <labelimage.h>
#include <historique.h>


QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QToolButton;
class Historique;

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

    Historique *historique;

signals:
    void enable_undo_redo();

public slots:
    void changer_image(QImage);
    void changer_image_sans_save(QImage);



};

#endif // ZONEDESSIN_H
