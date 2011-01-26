#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QtGui>

class Historique : public QObject
{
    Q_OBJECT
signals:
    void changer_image(QImage);

public:
    Historique();
    void undo();
    void redo();
    bool can_undo();
    bool can_redo();
    void init_historique(QImage);
    void ajouter_image(QImage);


private:
    QLinkedList<QImage> listeHistorique;
    int index;

};

#endif // HISTORIQUE_H
