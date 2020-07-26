#include "terrain.h"
#include <QDebug>

//void Terrain::clean_fluid() {
//    for (short i = 0; i < 9; i++) {
//        for (short j = 0; j < 13; j++) {
//            if ((tiles[i][j] != nullptr) and (tiles[i][j]->get_type() != 1)) {
//                delete tiles[i][j];
//                tiles[i][j] = nullptr;
//            }
//        }
//    }
//}

void Terrain::update_terrain_matrix(std::array<std::string, 9> *terrain_matrix) {
    for (short i = 0; i < 9; i++) {
        (*terrain_matrix)[i] = "";
        for (short j = 0; j < 13; j++) {
            if (tiles[i][j] != nullptr) (*terrain_matrix)[i].push_back(char(tiles[i][j]->get_type() + 48));
            else (*terrain_matrix)[i].push_back('0');
        }
    }
}

Terrain::Terrain(QGraphicsScene *_level, std::array<std::string, 9> *terrain_matrix, bool tutorial_level) :
                 level(_level) {

    rocks_num = 0;

    if (tutorial_level) {
        for (short i = 0; i < 9; i++) {
            for (short j = 0; j < 13; j++) tiles[i][j] = nullptr;
        }
    }
    else {
        for (short i = 0; i < 9; i++) {
            for (short j = 0; j < 13; j++) {
                if ((*terrain_matrix)[i][j] != '0') {
                    tiles[i][j] = new TerrainObject(i, j, short((*terrain_matrix)[i][j]) - 48);
                    if ((*terrain_matrix)[i][j] == '1') rocks_num++;
                }
                else tiles[i][j] = nullptr;
            }
        }
    }
}

Terrain::~Terrain() {
    for (short i = 0; i < 9; i++) {
        for (short j = 0; j < 13; j++) {
            if (tiles[i][j] != nullptr) {
                level->removeItem(tiles[i][j]);
                delete tiles[i][j];
            }
        }
    }
}
