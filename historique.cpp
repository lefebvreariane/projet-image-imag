#include "historique.h"

Historique::Historique(){

    index = 0;
}


void Historique::init_historique(QImage img){
    listeHistorique.clear();
    listeHistorique.append(img);

    index=1;

}

void Historique::ajouter_image(QImage img){
    //On supprime toute la fin
    QMutableLinkedListIterator<QImage> iterateur(listeHistorique);
    for(int i=0; i<index; i++)
        iterateur.next();

    while (iterateur.hasNext()) {
        iterateur.next();
        iterateur.remove();
    }

    //On ajoute l'image a la liste et repositionne l'index a la fin;
    listeHistorique.append(img);
    index++;
}

void Historique::undo(){
    index--;
    QMutableLinkedListIterator<QImage> iterateur(listeHistorique);
    for(int i=0; i<index; i++)
        iterateur.next();
    emit changer_image(iterateur.previous());
}

void Historique::redo(){

    QMutableLinkedListIterator<QImage> iterateur(listeHistorique);
    for(int i=0; i<index; i++)
        iterateur.next();
    index++;
    emit changer_image(iterateur.next());

}

bool Historique::can_undo(){
    if(index > 1 )
        return true;
    else
        return false;
}

bool Historique::can_redo(){
    if(index < listeHistorique.size() )
        return true;
    else
        return false;
}



