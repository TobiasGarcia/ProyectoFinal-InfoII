#include "game.h"
#include <QDebug>

void Game::create_new_game(std::string password) {

    //-----------------------------------Para este momento ya hemos verificado que el archivo y users.txt exista

    std::string data, row;

    data = game_id + ':' + password + '\n';
    std::fstream file(path + "users.txt", std::ios::app);
    file << data;
    file.close();

    //Apesar de que basta inicializar las variables y luego llamar al método save_game(),
    //como este utiliza tres ciclos for anidados, el costo computacional es mucho más
    //bajo si dentro de esta misma función escribismo el archivo al tiempo que
    //inicializamos las variables.

    //Últimos tres dígitos son las oleadas en que están los niveles,
    //el 0 es para identificar que el nivel está bloqueado.

    data = two_players?'1':'0';
    data.append("000100\n\n");

    rocks_num = 0;
    fluids_num = 0;
    extra_life = 0;
    levels_waves[0] = 1;
    levels_waves[1] = 0;
    levels_waves[2] = 0;

    //Llenamos las matrices de los 4 niveles.

    row = "0000000000000";
    for (short k = 0; k < 3; k++) {
        for (short i = 0; i < 9; i++) {
            //13 ceros.
            data.append(row);
            data.push_back('\n');
            terrain_matrices[k][i] = row;
        }
        data.push_back('\n');
    }

    file.open(path + "games_data/" + game_id + ".txt", std::ios::out);
    file << data;
    file.close();
}

void Game::save_game() {

    std::string data = "";

    data.push_back(two_players?'1':'0');
    data.push_back(char(rocks_num + 48));
    data.push_back(char(fluids_num + 48));
    data.push_back(extra_life?'1':'0');
    data.push_back(char(levels_waves[0] + 48));
    data.push_back(char(levels_waves[1] + 48));
    data.push_back(char(levels_waves[2] + 48));

    for (short k = 0; k < 3; k++) {
        for (short i = 0; i < 9; i++) {
            for (short j = 0; j < 13; j++) {
                data.push_back(char(terrain_matrices[k][i][j] + 48));
            }
            data.push_back('\n');
        }
        data.push_back('\n');
    }

    std::fstream file(path + "games_data/" + game_id + ".txt", std::ios::out);
    file << data;
    file.close();
}

void Game::load_game() {

    //-----------------------------------Para este momento ya hemos verificado que el archivo y users.txt exista

    std::string first_line;
    std::fstream file(path + "games_data/" + game_id + ".txt", std::ios::in);

    getline(file, first_line);

    for (short k = 0; k < 3; k++) {
        file.get();
        for (short i = 0; i < 9; i++) getline(file, terrain_matrices[k][i]);
    }

    file.close();

    two_players = (first_line[0] == '1');
    rocks_num = short(first_line[1]) - 48;
    fluids_num = short(first_line[2]) - 48;
    extra_life = (first_line[3] == '1');
    levels_waves[0] = short(first_line[4]) - 48;
    levels_waves[1] = short(first_line[5]) - 48;
    levels_waves[2] = short(first_line[6]) - 48;
}

Game::Game(std::string _path, QGraphicsView *_game_gv, bool _two_players, std::string _game_id, std::string password) :
    path(_path), game_id(_game_id), two_players(_two_players), game_gv(_game_gv) {

    if (password != "") create_new_game(password);
    else load_game();

    levels_menu = new LevelsMenu(two_players);
    connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
    game_gv->setScene(levels_menu);
}

void Game::level_selceted(short level_num) {
    delete levels_menu;
    if (level_num == 0) qDebug() << "Esto no está listo aún.";
    else {
        level = new Level(path, two_players, level_num, (levels_waves + (level_num - 1)),
                          &rocks_num, &fluids_num, &extra_life, &(terrain_matrices[level_num - 1]));
        game_gv->setScene(level);
    }
}














































