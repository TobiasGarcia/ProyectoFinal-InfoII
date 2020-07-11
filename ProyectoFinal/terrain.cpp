#include "terrain.h"

Terrain::Terrain() {

    for (short i = 0; i < 10; i++) tiles[i].fill(0);

    for (short i = 1; i < 4; i++) tiles[i + 2][3] = 1;
    for (short i = 1; i < 4; i++) tiles[i + 2][9] = 1;
    for (short j = 0; j < 5; j++) tiles[2][j + 4] = 1;
    for (short j = 0; j < 5; j++) tiles[6][j + 4] = 1;

    //tiles[6][5] = 0;
    //tiles[6][7] = 0;
    //tiles[6][6] = 0;
    tiles[5][3] = 0;
    tiles[3][3] = 0;

    //tiles[2][4] = 0;
    tiles[4][9] = 0;

    tiles[4][10] = 3;
    tiles[3][11] = 3;
    tiles[4][11] = 3;
    //tiles[3][9] = 0;
    //tiles[5][3] = 0;

    //tiles[6][7] = 0;

    //tiles[2][5] = 0;

    //tiles[3][9] = 0;
    //tiles[4][9] = 0;
    //tiles[5][9] = 0;
}
