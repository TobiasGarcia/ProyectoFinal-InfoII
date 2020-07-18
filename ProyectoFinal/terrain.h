#ifndef TERRAIN_H
#define TERRAIN_H

#include <array>
#include <terrainobject.h>
#include <QGraphicsScene>

//Clase Terrain para que pueda tener sus propios métodos.

class Terrain {
public:
    //0: Nada
    //1: Roca
    //2: Caracol
    //3: Fluido
    QGraphicsScene *level;
    std::array<std::array<TerrainObject*, 13>, 10> tiles;

    Terrain(QGraphicsScene *_level);
    ~Terrain();
};

#endif // TERRAIN_H
