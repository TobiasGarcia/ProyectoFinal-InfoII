#include "terrain.h"
#include <QDebug>

void Terrain::clean_fluid() {
    for (short i = 0; i < 10; i++) {
        for (short j = 0; j < 13; j++) {
            if ((tiles[i][j] != nullptr) and (tiles[i][j]->get_type() != 1)) {
                delete tiles[i][j];
                tiles[i][j] = nullptr;
            }
        }
    }
}

Terrain::Terrain(QGraphicsScene *_level) : level(_level) {

    for (short i = 0; i < 10; i++) tiles[i].fill(nullptr);

//    for (short i = 3; i < 6; i++) {
//        tiles[i][3] = new TerrainObject(i, 3, 1);
//        tiles[i][9] = new TerrainObject(i, 9, 1);
//    }

//    for (short j = 4; j < 9; j++) {
//        tiles[2][j] = new TerrainObject(2, j, 1);
//        tiles[6][j] = new TerrainObject(6, j, 1);
//    }

    //tiles[6][5] = 0;
    //tiles[6][7] = 0;
    //tiles[6][6] = 0;

//    delete tiles[6][8];
//    tiles[6][8] = nullptr;

//    delete tiles[2][4];
//    tiles[2][4] = nullptr;

//    delete tiles[5][3];
//    tiles[5][3] = nullptr;

    rocks_num = 0;

//    delete tiles[2][7];
//    tiles[2][7] = nullptr;

//    //tiles[2][4] = 0;
//    delete tiles[4][9];
//    delete tiles[6][4];

//    tiles[3][2] = 3;
//    tiles[3][1] = 3;
//    tiles[3][11] = 3;
//    tiles[4][11] = 3;
    //tiles[3][9] = 0;
    //tiles[5][3] = 0;

    //tiles[6][7] = 0;

    //tiles[2][5] = 0;

    //tiles[3][9] = 0;
    //tiles[4][9] = 0;
    //tiles[5][9] = 0;
}

Terrain::~Terrain() {
    for (short i = 0; i < 10; i++) {
        for (short j = 0; j < 13; j++) {
            if (tiles[i][j] != nullptr) {
                level->removeItem(tiles[i][j]);
                delete tiles[i][j];
            }
        }
    }
}
