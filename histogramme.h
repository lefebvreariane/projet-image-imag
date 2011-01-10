#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include <QWidget>

class Histogramme : public QWidget
{
    Q_OBJECT
public:
    explicit Histogramme(QImage i,QWidget *parent = 0);
    void paintEvent(QPaintEvent * event);

signals:

public slots:
private:
    QImage image;
    int maxi(int* tab, int taille);

};

#endif // HISTOGRAMME_H
