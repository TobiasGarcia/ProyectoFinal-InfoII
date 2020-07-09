#include "level.h"

Level::Level() {

    setSceneRect(0, 0, 794, 599); //795x600 pixeles para que los jugadores se muevan de 15 en 15.

    players = new Players(390, 285, 495, 285); //Colocarlos en múltiplos de 15.

    players->setFlag(QGraphicsItem::ItemIsFocusable);
    players->setFocus();

    addItem(players);
    addItem(players->multi);
}

Level::~Level() {
    delete players;
}
