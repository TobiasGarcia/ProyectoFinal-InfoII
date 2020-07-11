#include "level.h"
#include "QDebug"

Level::Level() {

    srand(time(nullptr));

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.

    terrain = new Terrain;
    display_terrain();

    carlos = new Enemy(3, -1, terrain);
    addItem(carlos);

    carlos = new Enemy(-1, 6, terrain);
    addItem(carlos);

    carlos = new Enemy(9, 4, terrain);
    addItem(carlos);

    carlos = new Enemy(0, 13, terrain);
    addItem(carlos);

    terrain->tiles[4][9] = 1;
    terrain->tiles[3][3] = 1;


//    carlos = new Enemy(439, 270, terrain);
//    addItem(carlos);

//    carlos = new Enemy(330, 161, terrain);
//    addItem(carlos);

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

    TerrainObject *terrain_object;

    for (short i = 0; i < 10; i++) for (short j = 0; j < 13; j++) {
        if (terrain->tiles[i][j]) {
            terrain_object = new TerrainObject(60*j, 60*i, terrain->tiles[i][j]);
            addItem(terrain_object);
        }
    }
    terrain_object = new TerrainObject(60*9, 60*4, 1);
    addItem(terrain_object);

    terrain_object = new TerrainObject(60*3, 60*3, 1);
    addItem(terrain_object);

//    QGraphicsLineItem *line;
//    for (short i = 0; i < 9; i++) {
//        line = new QGraphicsLineItem(0, 60*(i + 1) - 1, 779, 60*(i + 1) - 1);
//        addItem(line);
//    }
//    for (short j = 0; j < 12; j++) {
//        line = new QGraphicsLineItem(60*(j + 1) - 1, 0, 60*(j + 1) - 1, 599);
//        addItem(line);
//    }
}





















































































