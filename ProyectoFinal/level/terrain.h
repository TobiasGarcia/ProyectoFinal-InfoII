#ifndef TERRAIN_H
#define TERRAIN_H

#include <array>

#include <QGraphicsScene>

#include "level/terrainobject.h"

//Esta clase es posee muy poco código, pero resulta muy útil para estructurar el programa
//y dividir las tareas. Modela una forma de almacenar los objetos de la clase
//TerrainObjet que se agregan al terreno del mapa.

class Terrain {
private:
    QGraphicsScene *level;

public:

    //La variable rocks_num indica la cantidad de rocas que se encuentran en el terreno,
    //como necesitamos modificarla y leerla desde fuera de la clase, la colocamos
    //como atributo público.

    short rocks_num;

    //La matriz tiles almacenará los punteros hacia los objetos de la clase TerrainObjet
    //los cuales se encontrarán en el mapa, utilizamos el contenedor array pues la
    //matriz de las casillas del mapa siempre tiene un tamaño fijo.

    std::array<std::array<TerrainObject*, 13>, 10> tiles;

    void update_terrain_matrix(std::array<std::string, 9> *terrain_matrix);

    Terrain(QGraphicsScene *_level);
    Terrain(QGraphicsScene *_level, std::array<std::string, 9> *terrain_matrix);
    ~Terrain();

};

#endif // TERRAIN_H
