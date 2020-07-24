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
    bool two_players, extra_life;
    short rocks_num, fluids_num, levels_waves[3];
    std::array<std::array<std::string, 9>, 3> terrain_matrices;
    QGraphicsView *game_gv;

    void create_new_game(std::string password);
    void save_game();
    void load_game();

public:
    Game(std::string _path, QGraphicsView *_game_gv, bool _two_players, std::string _game_id, std::string password = "");
public slots:
    void level_selceted(short level_num);
};

#endif // GAME_H
