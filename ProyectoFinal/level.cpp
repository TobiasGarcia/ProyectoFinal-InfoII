#include "level.h"
#include "QDebug"

Level::Level() {

    srand(time(nullptr));

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.

    terrain = new Terrain;
    display_terrain();

    carlos = new Enemy;
    addItem(carlos);

    players = new Players(390, 285, 495, 285); //Colocarlos en múltiplos de 15.

    players->setFlag(QGraphicsItem::ItemIsFocusable);
    players->setFocus();

    addItem(players->multi);
    addItem(players);
}

Level::~Level() {
    delete carlos;
    delete players;
    delete terrain;
}

void Level::display_terrain() {

    QPixmap *pix = new QPixmap(":/images/resources/images/roca.png");
    QGraphicsPixmapItem *rock;

    for (short i = 0; i < 10; i++) {
        for (short j = 0; j < 13; j++) {
            if (terrain->grid.at(i).at(j) == 4) {
                *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
                rock = new QGraphicsPixmapItem(*pix);
                rock->setPos(60*j, 60*i);
                addItem(rock);
            }
        }
    }

    QGraphicsLineItem *line;
    for (short i = 0; i < 9; i++) {
        line = new QGraphicsLineItem(0, 60*(i + 1) - 1, 779, 60*(i + 1) - 1);
        addItem(line);
    }
    for (short j = 0; j < 12; j++) {
        line = new QGraphicsLineItem(60*(j + 1) - 1, 0, 60*(j + 1) - 1, 599);
        addItem(line);
    }
}





















































































