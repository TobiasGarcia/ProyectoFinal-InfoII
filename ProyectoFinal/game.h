#ifndef GAME_H
#define GAME_H

#include "level/terrain.h"
#include "level/level.h"
#include "levels_menu/levelsmenu.h"
#include "minigame/minigame.h"
#include <QGraphicsView>
#include <fstream>
#include <array>

class Game: public QObject {

    Q_OBJECT

private:

    LevelsMenu *levels_menu;
    Level *level;
    Minigame *minigame;
    std::string path, game_id;
    bool two_players, extra_life, winner;
    short rocks_num, fluids_num, levels_waves[3], healths[3], current_level;
    std::array<std::array<std::string, 9>, 3> terrain_matrices;
    QGraphicsView *game_gv;

    void create_new_game(std::string password);
    void save_game();
    void load_game();
    void clear_terrain(std::array<std::string, 9> *terrain_matrix);

public:
    Game(std::string _path, QGraphicsView *_game_gv, std::string _game_id,
         std::string password = "", bool _two_players = false);
public slots:
    void level_selceted(short level_num);
    void save_game_slot(bool level_finished);
    void minigame_finished();
signals:
    void exit_game();
};

#endif // GAME_H
