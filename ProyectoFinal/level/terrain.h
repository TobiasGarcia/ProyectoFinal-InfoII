#ifndef TERRAIN_H
#define TERRAIN_H

#include <array>
#include "level/terrainobject.h"
#include <QGraphicsScene>

//Clase Terrain para que pueda tener sus propios métodos.

class Terrain {
private:
    QGraphicsScene *level;
public:
    //0: Nada
    //1: Roca
    //2: Caracol
    //3: Fluido

    //Público porque hay que sumarle y leer el valor.

    short rocks_num;
    std::array<std::array<TerrainObject*, 13>, 10> tiles;
    void clean_fluid();

    Terrain(QGraphicsScene *_level);
    ~Terrain();
};

#endif // TERRAIN_H
