#ifndef GAME_H
#define GAME_H

#include <array>
#include <fstream>
#include <QGraphicsView>

#include "levels_menu/levelsmenu.h"
#include "minigame/minigame.h"
#include "level/terrain.h"
#include "level/level.h"

class Game: public QObject {

    Q_OBJECT

private:
    QGraphicsView *game_gv;

    Level *level;
    Minigame *minigame;
    LevelsMenu *levels_menu;

    std::string path;
    short current_level;

    //La información necesaria para definir el estado de la partida es:

    //El nombre de la partida:                                     game_id
    //El modo de juego, de un solo jugador o multijugador:     two_players
    //Si ya se han superado todos los niveles:                      winner
    //El número de rocas que se posee:                           rocks_num
    //El número de pegamentos que se posee:                      glues_num
    //Si se posee una vida extra:                               extra_life
    //El número de la oleada en que se dejó cada nivel:       levels_waves
    //La vida de las bases de cada nivel:                          healths
    //Los terrenos de cada nivel:                         terrain_matrices

    //El tipo de la mayoria de variables no requiere explicación, pues son consecuentes,
    //pero es bueno detallar el tipo de las últimas tres:

    //El juego contará con 4 niveles, de los cuales uno es a manera de tutorial, por lo
    //cual de este no debemos guardar información, de ahí es que levels_waves, healths
    //y terrain_matrices son arreglos de 3 elementos en lugar de 4 elementos.

    //La escena de los niveles se encuentra dividida internamente en una matriz 9 x 13, donde
    //en cada casilla se pueden haber uno de tres objeto: pegamento, pegamento de caracol o
    //una roca, como veremos más adelante el pegmente se puede colocar en cualquier lugar
    //menos en las casilla centrla, el pegamento de caracol lo coloca un enemigo que es
    //un caracol a medida que se desplaza por el mapa, y se pueden colocar sólo en los
    //alrededores de la base, en donde esta última es el objeto que deben defender
    //los jugadores como se especificó en el documento de la descripción del
    //proyecto.

    //La variable terrain_matrices se utiliza para almacenar los objetos que se encuentran
    //en los terrenos de los 3 nieveles distintos al tutorial (de ahora en adelante nos referiremos
    //a estos 3 niveles como "los niveles", es decir, el tutorial técnicamente es un nivel, pero no
    //lo consideraremos cuando nos referimos a ellos, a menos que se especifique lo contrario),
    //cada matriz posee 9 strings de la forma "ddddddddddddd" (trece ds), donde cada d es un
    //dígito del 0 al 3, y representa qué hay en esa posición del mapa:

    //0: No hay nada.
    //1: Hay una roca.
    //2: Hay pegamento.
    //3: Hay pegamento de caracol.

    std::string game_id;
    bool two_players, extra_life, winner;
    short rocks_num, glues_num, levels_waves[3], healths[3];
    std::array<std::array<std::string, 9>, 3> terrain_matrices;

    void save_game();
    void load_game();
    void create_new_game(std::string password);
    void clear_terrain(std::array<std::string, 9> *terrain_matrix);

public:
    Game(std::string _path, QGraphicsView *_game_gv, std::string _game_id,
         std::string password = "", bool _two_players = false);

public slots:
    void minigame_finished();
    void level_selceted(short level_num);
    void update_level_progress(short progress_type);

signals:
    void exit_game();
};

#endif // GAME_H
