#ifndef GAME_H
#define GAME_H

#include "level/terrain.h"
#include <fstream>
#include <array>

class Game {
private:
    std::string path, game_id;
    bool two_players, extra_life;
    short rocks_num, fluids_num, levels_waves[3];
    std::array<std::array<std::string, 9>, 3> terrain_matrices;

    void create_new_game(std::string password);
    void save_game();
    void load_game();

public:
    Game(std::string _path, bool _two_players, std::string _game_id, std::string password = "");
};

#endif // GAME_H
