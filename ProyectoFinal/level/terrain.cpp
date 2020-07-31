#include "terrain.h"

Terrain::Terrain(QGraphicsScene *_level, std::array<std::string, 9> *terrain_matrix) : level(_level) {

    //La variable _level apunta a la escena del nivel o el tutorial, y la utilizamos para poder eliminar
    //los TerrainObjets que se encuentren en esta; la variable terrain_matrix es la matriz del terreno
    //con el que se retomará el nivel, esta matriz tiene el formato que utiliza la clase Game para
    //cargar y guardar los terrenos de los diferentes niveles.

    //Comenzamos colocan la variable rocks_num en 0, para irla aumentando cada vez que coloquemos
    //una roca.

    rocks_num = 0;
    for (short i = 0; i < 9; i++) {
        for (short j = 0; j < 13; j++) {

            //Iteramos sobre los elementos de terrain_matrix, los cuales son strings, que mediante
            //el ciclo interior, también recorremos.

            //Recordemos que la correspondencia entre dígitos del string y los objetos del terreno
            //es la siguiente: 0 cuando no hay nada, 1 para indicar que hay una roca, 2 para
            //indicar que hay un pegamento de caracol, y 3 para pegamento colocado por
            //el jugador.

            if ((*terrain_matrix)[i][j] != '0') {

                //Si hay un objeto, construimos un TerrainObject para este y almacenamos
                //su dirección de memoria en la matriz tiles.

                tiles[i][j] = new TerrainObject(i, j, short((*terrain_matrix)[i][j]) - 48);

                //En caso de ser una roca, aumentamos el valor de la variable rocks_num.

                if ((*terrain_matrix)[i][j] == '1') rocks_num++;
            }

            //Si lo que hay en el string es un 0, colocamos ese elemento de tiles apuntado
            //a nullptr.

            else tiles[i][j] = nullptr;
        }
    }
}

Terrain::Terrain(QGraphicsScene *_level) : level(_level){

    //Este es un constructor sobrecargado, y es utilizado para inicializar la matriz tiles para
    //el caso particular del tutorial, pues como durante este no se puede guardar la partida,
    //siempre estará vacío.

    rocks_num = 0;
    for (short i = 0; i < 9; i++) {
        for (short j = 0; j < 13; j++) tiles[i][j] = nullptr;
    }
}

Terrain::~Terrain() {

    //Liberamos la memoria ocupada por los TerrainObjets que haya en la escena.

    for (short i = 0; i < 9; i++) {
        for (short j = 0; j < 13; j++) {
            if (tiles[i][j] != nullptr) {
                level->removeItem(tiles[i][j]);
                delete tiles[i][j];
            }
        }
    }
}

void Terrain::update_terrain_matrix(std::array<std::string, 9> *terrain_matrix) {

    //El propósito de este método es actualizar los valores de la matriz del terreno (la que contiene los
    //strings de acuerdo al formato utilizado por la clase Game), con la información de la matriz tiles.

    for (short i = 0; i < 9; i++) {

        //Colocamos el string vacío y le vamos agregando los objetos.

        (*terrain_matrix)[i] = "";
        for (short j = 0; j < 13; j++) {

            //En caso de que haya un TerrainObjet, colocamos el dígito correspondiente en el string,
            //o 0 en caso contrario.

            if (tiles[i][j] != nullptr) (*terrain_matrix)[i].push_back(char(tiles[i][j]->get_type() + 48));
            else (*terrain_matrix)[i].push_back('0');
        }
    }
}
