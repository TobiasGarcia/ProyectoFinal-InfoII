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
    data.append("000000\n\n");

    rocks_num = 0;
    fluids_num = 0;
    extra_life = 0;
    levels_waves[0] = 0;
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

    if (levels_waves[0] == 0) {

        //Sí se acaba de crear la cuenta, es decir, no se ha desbloqueado el primer nivel (levels_waves[0] == 0),
        //no comenzamos en el menú de niveles sino en el nivel tutorial.

        //Le pasamos los argumentos del primer nivel, pues esto no afecta ya que el constructor de la
        //clase Level realiza los cambios necesarios en caso de ser nivel tutorial.

        current_level = 0;
        level = new Level(path, two_players, current_level, levels_waves[0],
                          &rocks_num, &fluids_num, &extra_life, healths, &(terrain_matrices[0]));
        connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
        game_gv->setScene(level);
    }
    else {
        levels_menu = new LevelsMenu(two_players, rocks_num, fluids_num, extra_life, levels_waves, winner);
        connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
        game_gv->setScene(levels_menu);
    }
}

void Game::level_selceted(short level_num) {

    delete levels_menu;
    if (level_num == -1) emit exit_game();
    else {
        current_level = level_num;

        if (current_level == 0) {

            //Le pasamos los argumentos del primer nivel, pues esto no afecta ya que el constructor de la
            //clase Level realiza los cambios necesarios en caso de ser nivel tutorial.

            level = new Level(path, two_players, current_level, levels_waves[0],
                              &rocks_num, &fluids_num, &extra_life, healths, &(terrain_matrices[0]));
            connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
            game_gv->setScene(level);
        }
        else {
            //Como terrain_matrices es un std::array y no un array de la forma [],
            //para pasar la dirección de memoria de alguno de sus elemetnos
            //hay que utilizar &(terrain_matrices[i]), en lugar de
            //simplemente (terrain_matrices + i).

            level = new Level(path, two_players, current_level, levels_waves[level_num - 1],
                              &rocks_num, &fluids_num, &extra_life, (healths + (level_num - 1)),
                              &(terrain_matrices[level_num - 1]));
            connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
            game_gv->setScene(level);
        }
    }
}

void Game::update_level_progress(short progress_type) {

    if (progress_type == 3) {
        delete level;

        //Si no es el nivel tutorial, limpiamos el terreno, en caso contrario,
        //cargamos la partida para recuperar el estado anterior.

        if (current_level != 0) {
            //Como terrain_matrices es un std::array y no un array de la forma [],
            //para pasar la dirección de memoria de alguno de sus elemetnos
            //hay que utilizar &(terrain_matrices[i]), en lugar de
            //simplemente (terrain_matrices + i).

            clear_terrain(&(terrain_matrices[current_level - 1]));
            levels_waves[current_level - 1] = 1;
            healths[current_level - 1] = 1000;
        }
        else load_game();

        //Desbloqueamos el siguiente nivel

        if ((current_level < 3) and (levels_waves[current_level] == 0)) levels_waves[current_level] = 1;
        else if (current_level == 3) winner = true;

        minigame = new Minigame(two_players, &rocks_num, &fluids_num, &extra_life);
        connect(minigame, &Minigame::minigame_finished, this, &Game::minigame_finished);
        game_gv->setScene(minigame);
    }
    else if (progress_type == 2) {

        //En este punto la matriz de terreno de std::strings está actualizada
        //con la info del terreno de la partida como tal.

        levels_waves[current_level - 1]++;
        save_game();
    }
    else {
        delete level;

////        En este punto la matriz de terreno de std::strings NO está actualizada
////        con la info del terreno de la partida como tal, por lo cual, como
////        queremos que el jugador pueda retornar con el anterior terreno,
////        el hecho que no esté actualizada es necesario.

////        La oleada tampoco se actualizó, por lo cual regresará a donde estaba.

////        Colocamos la salud de la base de nuevo en 1000, para que cuando regrese
////        de nuevo al nivel tenga una ventaja, pues la base poseerá una vida
////        mayor o igual a la que tenía cuando inició la oleada en que fue
////        derrotado.

        load_game();

        //En caso de que del nivel se haya salido porque la base fue destruida, le ayudamos un poco
        //al jugador de caso de que la vida de la base haya quedado muy baja cuando se guardó la
        //partida, haciendo que cuando vuevla a entrar al nivel la vida de la base será el
        //máximo entre la vida que tenía y 500.

        if ((progress_type == 0) and (healths[current_level - 1] < 500)) {
            healths[current_level - 1] = 500;
            save_game();
        }

        //Volvemos al menú de niveles.

        levels_menu = new LevelsMenu(two_players, rocks_num, fluids_num, extra_life, levels_waves, winner);
        connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
        game_gv->setScene(levels_menu);
    }
}

void Game::minigame_finished() {
    delete minigame;
    save_game();

    levels_menu = new LevelsMenu(two_players, rocks_num, fluids_num, extra_life, levels_waves, winner);
    connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
    game_gv->setScene(levels_menu);
}














































