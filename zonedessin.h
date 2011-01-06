#ifndef ZONEDESSIN_H
#define ZONEDESSIN_H

#include <QPainter>
#include <QWidget>

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
    QImage resultImage;
    QPainter *painter;
    QLabel *resultLabel;

    void afficher_image();
    void ecrire_image();

signals:

public slots:

};

#endif // ZONEDESSIN_H
