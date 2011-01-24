#ifndef FENETREFILTRES_H
#define FENETREFILTRES_H

#include <QtGui>


class FenetreFiltres : public QWidget
{
    Q_OBJECT
public:
    explicit FenetreFiltres(QWidget *parent = 0);

signals:

public slots:
    void clic_ok();
    void clic_annuler();
};

#endif // FENETREFILTRES_H
