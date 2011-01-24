#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include <QtGui>

class Histogramme : public QWidget
{
    Q_OBJECT
public:
    explicit Histogramme(QWidget *parent = 0);
    void paintEvent(QPaintEvent * event);
    QImage image;
    int couche;

signals:

public slots:
private:
    void calcul_histo(int *tab, QColor c);
    int maxi(int* tab, int taille);

};

#endif // HISTOGRAMME_H
