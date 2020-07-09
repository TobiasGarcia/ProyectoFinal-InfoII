#ifndef TERRAIN_H
#define TERRAIN_H

#include <array>

//Clase Terrain para que pueda tener sus propios métodos.

class Terrain {
private:
    //0: Nada
    //1: Pared
    //2: Ralentizador
    std::array<std::array<short, 53>, 40> grid;
public:
    Terrain();
};

#endif // TERRAIN_H
