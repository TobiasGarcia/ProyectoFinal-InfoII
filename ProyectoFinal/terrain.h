#ifndef TERRAIN_H
#define TERRAIN_H

#include <array>

//Clase Terrain para que pueda tener sus propios métodos.

class Terrain {
public:
    //0: Nada
    //1: Roca
    //2: Caracol
    //3: Ralentizador
    std::array<std::array<short, 13>, 10> tiles;

    Terrain();
};

#endif // TERRAIN_H
