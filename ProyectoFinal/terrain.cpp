#include "terrain.h"

Terrain::Terrain() {

    for (short i = 0; i < 10; i++) grid.at(i).fill(0);


    for (short i = 1; i < 4; i++) grid.at(i + 2).at(3) = 4;
    for (short i = 1; i < 4; i++) grid.at(i + 2).at(9) = 4;
    for (short j = 0; j < 5; j++) grid.at(2).at(j + 4) = 4;
    for (short j = 0; j < 5; j++) grid.at(6).at(j + 4) = 4;
}
