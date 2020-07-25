#include "game.h"
#include <QDebug>

void Game::create_new_game(std::string password) {

    //-----------------------------------Para este momento ya hemos verificado que el archivo y users.txt exista

    std::string data, row;

    //Hay que dejar un \n al final de users.txt.

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

    data = '0';
    winner = false;

    data.push_back(two_players?'1':'0');
    data.append("000100\n\n");

    rocks_num = 0;
    fluids_num = 0;
    extra_life = 0;
    levels_waves[0] = 1;
    levels_waves[1] = 0;
    levels_waves[2] = 0;
    healths[0] = 1000;
    healths[1] = 1000;
    healths[2] = 1000;

    //Llenamos las matrices de los 4 niveles.

    row = "0000000000000";
    for (short k = 0; k < 3; k++) {
        data.append("1000\n");
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

    data.push_back(winner?'1':'0');
    data.push_back(two_players?'1':'0');
    data.push_back(char(rocks_num + 48));
    data.push_back(char(fluids_num + 48));
    data.push_back(extra_life?'1':'0');
    data.push_back(char(levels_waves[0] + 48));
    data.push_back(char(levels_waves[1] + 48));
    data.push_back(char(levels_waves[2] + 48));

    data.append("\n\n");

    for (short k = 0; k < 3; k++) {
        data.append(std::to_string(healths[k]));
        data.push_back('\n');
        for (short i = 0; i < 9; i++) {
            for (short j = 0; j < 13; j++) {
                data.push_back(terrain_matrices[k][i][j]);
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

    std::string first_line, line;
    std::fstream file(path + "games_data/" + game_id + ".txt", std::ios::in);

    getline(file, first_line);

    for (short k = 0; k < 3; k++) {
        file.get();
        getline(file, line);
        healths[k] = stoi(line);
        for (short i = 0; i < 9; i++) getline(file, terrain_matrices[k][i]);
    }

    file.close();

    winner = (first_line[0] == '1');
    two_players = (first_line[1] == '1');
    rocks_num = short(first_line[2]) - 48;
    fluids_num = short(first_line[3]) - 48;
    extra_life = (first_line[4] == '1');
    levels_waves[0] = short(first_line[5]) - 48;
    levels_waves[1] = short(first_line[6]) - 48;
    levels_waves[2] = short(first_line[7]) - 48;
}

void Game::clear_terrain(std::array<std::string, 9> *terrain_matrix) {
    for (short i = 0; i < 9; i++) (*terrain_matrix)[i] = "0000000000000";
}

Game::Game(std::string _path, QGraphicsView *_game_gv, std::string _game_id,
           std::string password, bool _two_players) :

    path(_path), game_id(_game_id), two_players(_two_players), game_gv(_game_gv) {

    if (password != "") create_new_game(password);
    else load_game();

    levels_menu = new LevelsMenu(two_players, rocks_num, fluids_num, extra_life, levels_waves, winner);
    connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
    game_gv->setScene(levels_menu);
}

void Game::level_selceted(short level_num) {

    delete levels_menu;
    if (level_num == -1) emit exit_game();
    else {
        current_level = level_num;
        if (level_num == 0) qDebug() << "Esto no está listo aún.";
        else {

            //Como terrain_matrices es un std::array y no un array de la forma [],
            //para pasar la dirección de memoria de alguno de sus elemetnos
            //hay que utilizar &(terrain_matrices[i]), en lugar de
            //simplemente (terrain_matrices + i).

            level = new Level(path, two_players, level_num, levels_waves[level_num - 1],
                              &rocks_num, &fluids_num, &extra_life, (healths + (level_num - 1)),
                              &(terrain_matrices[level_num - 1]));
            connect(level, &Level::save_game, this, &Game::save_game_slot);
            game_gv->setScene(level);
        }
    }
}

void Game::save_game_slot(bool level_finished) {
    if (level_finished) {
        delete level;

        //Como terrain_matrices es un std::array y no un array de la forma [],
        //para pasar la dirección de memoria de alguno de sus elemetnos
        //hay que utilizar &(terrain_matrices[i]), en lugar de
        //simplemente (terrain_matrices + i).

        clear_terrain(&(terrain_matrices[current_level - 1]));
        levels_waves[current_level - 1] = 1;
        healths[current_level - 1] = 1000;

        //Desbloqueamos el siguiente nivel

        if ((current_level < 3) and (levels_waves[current_level] == 0)) levels_waves[current_level] = 1;
        else if (current_level == 3) {
            winner = true;
            //NO OLVIDAR GUARDAR EL JUEGO CUANDO LO TERMINEN.
            qDebug() << "¡Has terminado el juego!";
        }

        minigame = new Minigame(two_players, &rocks_num, &fluids_num, &extra_life);
        connect(minigame, &Minigame::minigame_finished, this, &Game::minigame_finished);
        game_gv->setScene(minigame);
    }
    else {
        levels_waves[current_level - 1]++;
        save_game();
    }
}

void Game::minigame_finished() {
    delete minigame;
    save_game();

    levels_menu = new LevelsMenu(two_players, rocks_num, fluids_num, extra_life, levels_waves, winner);
    connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
    game_gv->setScene(levels_menu);
}














































