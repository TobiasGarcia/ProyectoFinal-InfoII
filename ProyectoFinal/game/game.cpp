#include "game.h"

Game::Game(std::string _path, QGraphicsView *_game_gv, std::string _game_id,
           std::string password, bool _two_players) :

           game_gv(_game_gv), path(_path), game_id(_game_id), two_players(_two_players) {

    //La variable path almacena la ruta hasta la carpeta data, a la cual deben acceder varios métodos de esta
    //clase; el puntero _game_gv apunta al QGraphicsView de la interfaz gráfica; _game_id almacena el nombre
    //de la partida; password la respectiva contraseña de la partida, o "" en caso de que se esté cargando
    //la partida; _two_players indica si el juego se encuentra en modo de un solo jugador o multijugador.

    //Para reconocer si se está cargando o creando una nuva partida, utilizamos la variable
    //password que, como desde la interfaz gráfica habíamos verificado que no estuviera
    //vacía cuando se creara una partida nueva, si ésta lo está, es porque en realidad
    //se está cargando una partida antigua.

    if (password != "") create_new_game(password);
    else load_game();

    //Recordemos que levels_waves[0] almacena la oleada en que se encuentra el primer nivel,
    //o 0 cuando este aún no se ha desbloqueado, por lo cual si efectivamente es 0, en lugar
    //de enviar al usuario al menú de niveles, le enviamos al tutorial.

    if (levels_waves[0] == 0) {

        //Almacenamos en la variable current_level 0 pues vamos a ejecutar el tutorial.

        current_level = 0;

        //Construimos el nivel y lo colocamos en el QGraphicsView.

        level = new Level(path, two_players, current_level);
        connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
        game_gv->setScene(level);
    }
    else {

        //En caso de que ya se haya cursado el tutorial, es decir, levels_waves[0] posee un valor
        //distinto de 0 pues ya se ha desbloqueado el primer nivel, creamos el nivel y lo
        //colocamos en el QGraphicsView.

        levels_menu = new LevelsMenu(two_players, rocks_num, glues_num, extra_life, levels_waves, winner);
        connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
        game_gv->setScene(levels_menu);
    }
}

void Game::create_new_game(std::string password) {

    //Este método inicializa todas las variables del estado de la partida además de guardar esta información
    //en el .txt correspondiente; también añade la información de acceso de la partida al archivo users.txt.

    std::fstream file;
    std::string data, row;

    //Para este momento ya hemos verificado la existencia de los archivos de texto mediante el método
    //check_txt_files() de la clase Widget, por lo cual los podemos abrir sin preocupaciones.

    //El formato para guardar la información de acceso a las partidas dentro de users.txt es:

    //PARTIDA:CONTRASEÑA\n

    //Donde PARTIDA es el nombre de la partida y CONTRASEÑA la respectiva contraseña.

    //Obsérvese el '\n' al final de la línea, es por esto que es de vital importancia que si se va a modifar
    //algo del archivo users.txt, se deje un salto de línea al final de este, aunque si se elimina toda la
    //información no hay que agregar nada, simplemente se deja todo vacío.

    data = game_id + ':' + password + '\n';
    file.open(path + "users.txt", std::ios::app);
    file << data;
    file.close();

    //En lugar de inicializar todas las variables y luego llamar el método save_game(), es prefrerible
    //ir llenando a la par el string data con la información de una partida nueva, esto es para
    //no tener que iterar dos veces para inicializar, y luego guardar, la matriz del terreno,
    //por lo cual si vamos llenando el string data mientras inicializamos la matriz, podemos
    //utilizar un único ciclo for en el que se realicen ambas operaciones.

    //La información del estado de la partida se almacenará en un .txt dentro de la carpeta data/games_data,
    //bajo el nombre de la id de la partida; el formato en que se guardará la información será el siguiente:

    //FTRGEW1W2W3

    //HEALTH1
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd

    //HEALTH2
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd

    //HEALTH3
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd
    //ddddddddddddd

    //Donde en el esquema anterior F es un 1 o 0, representa si se han superado todos los niveles del juego,
    //1 para superados y 0 si falta por lo menos uno por superar; T es un 1 o 0, representa si la partida
    //es de multijugador o de un solo jugador, 1 para multijugador y 0 para uno solo; R es un dígito del
    //0 al 4, representa la cantidad de rocas que se posee; G es un dígito del 0 al 4, representa la
    //cantidad de pegamentos que se posee; E es un 1 o 0, representa si se cuenta con una vida extra
    //o no, 1 si se posee una vida extra o 0 cuando no; W1 es un dígito del 0 al 5, representa la
    //oleada en que se dejó el primer nivel, un valor entre el 1 y el 5, inclusive, significa
    //que se retoma el nivel desde esa oleada, un valor de 0 representa que ese nivel aún no
    //ha sido desbloqueado; W2 y W3 funcionan de forma análoga a W1 pero para los otros dos
    //niveles; HEALTH1 es un número entero desde el 1 hasta el 1000, que representa la
    //salud restante de la base del nivel 1, notemos que no puede ser 0 pues el nivel
    //ya se habría perdido; HEALTH2 y HEALTH3 funcionan de forma análoga a HEALTH1
    //pero para los otros dos niveles; y por último cada uno de los arreglos de
    //letras 'd' representa la matriz del terreno del nivel correspondiente,
    //donde cada d puede ser un dígito desde el 0 hasta el 3 como se
    //explicó en el .h de esta misma clase.

    //A continuación iniciaizamos las variables del estado de la partida y vamos carganado
    //esa misma información en el string data.

    data = '0';
    winner = false;

    data.push_back(two_players?'1':'0');
    data.append("000000\n\n");

    rocks_num = 0;
    glues_num = 0;
    extra_life = 0;

    //Todos los niveles comienzan bloqueados porque primero se debe
    //cursar el tutorial para desbloquear el primer nivel.

    levels_waves[0] = 0;
    levels_waves[1] = 0;
    levels_waves[2] = 0;

    healths[0] = 1000;
    healths[1] = 1000;
    healths[2] = 1000;

    //Inicializamos la matriz del terreno de los tres niveles y a la par
    //vamos agregando las filas de ceros al string data, pues tenemos
    //seguridad de que no hay objetos del terreno en ningún nivel
    //pues la partida se está creando.

    row = "0000000000000"; //13 ceros.

    for (short k = 0; k < 3; k++) {

        //El máximo de salud es 1000.
        data.append("1000\n");

        for (short i = 0; i < 9; i++) {
            data.append(row);
            data.push_back('\n');
            terrain_matrices[k][i] = row;
        }
        data.push_back('\n');
    }

    //Luego de ensamblar toda la información, abrimos el .txt y la guardamos.

    file.open(path + "games_data/" + game_id + ".txt", std::ios::out);
    file << data;
    file.close();
}

void Game::save_game() {

    //Este método se encarga de guardar la información del estado actual de
    //la partida en el .txt correspondiente.

    std::string data = "";

    //Añadimos la información a data según las variables del estado.

    data.push_back(winner?'1':'0');
    data.push_back(two_players?'1':'0');
    data.push_back(char(rocks_num + 48));
    data.push_back(char(glues_num + 48));
    data.push_back(extra_life?'1':'0');
    data.push_back(char(levels_waves[0] + 48));
    data.push_back(char(levels_waves[1] + 48));
    data.push_back(char(levels_waves[2] + 48));

    data.append("\n\n");

    //Añadimos la información de las matrices de los terrenos además de la
    //salud restante de cada base.

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

    //Abrimos el .txt que lleva el nombre de la partida y sobrescribimos la información.

    std::fstream file(path + "games_data/" + game_id + ".txt", std::ios::out);
    file << data;
    file.close();
}

void Game::load_game() {

    //Este método se encarga de cargar la información del estado actual de la partida desde
    //el .txt correspondiente.

    //Recordemos que para este momento ya hemos verificado la existencia de los archivos de
    //texto mediante el método check_txt_files() de la clase Widget.

    std::string first_line, line;
    std::fstream file(path + "games_data/" + game_id + ".txt", std::ios::in);

    //Tomamos la primera línea y la guardamos en first_line para poder procesarla posetriormente,
    //así podemos economizar en el tiempo que mantenemos el .txt abireto.

    getline(file, first_line);

    for (short k = 0; k < 3; k++) {

        //Leemos el caracter de salto de lína para poder pasar a la línea donde se encuentra
        //la salud restante de la base.

        file.get();

        //Tomamos la salud restante de la base y luego cargamos la matriz del terreno.

        getline(file, line);
        healths[k] = stoi(line);

        for (short i = 0; i < 9; i++) getline(file, terrain_matrices[k][i]);
    }

    file.close();

    //Procesamos la primera línea del .txt que habíamos almacenado en first_line,
    //para terminar de cargar toda la información del estado de la partida.

    winner = (first_line[0] == '1');
    two_players = (first_line[1] == '1');

    rocks_num = short(first_line[2]) - 48;
    glues_num = short(first_line[3]) - 48;
    extra_life = (first_line[4] == '1');

    levels_waves[0] = short(first_line[5]) - 48;
    levels_waves[1] = short(first_line[6]) - 48;
    levels_waves[2] = short(first_line[7]) - 48;
}

void Game::clear_terrain(std::array<std::string, 9> *terrain_matrix) {

    //Este método se encarga de limpiar toda la matriz del terreno de uno de
    //los nievles; esto resulta útil pues cuando se supera un nivel se debe
    //limpiar el terreno.

    for (short i = 0; i < 9; i++) (*terrain_matrix)[i] = "0000000000000";
}

void Game::level_selceted(short level_num) {

    //Este slot es activado por la señal level_selected() de la clase LevelsMenu, y su propósito es construir
    //y colocar el nivel seleccionado en el QGraphicsView, tambien puede suceder que se desee repetir el
    //tutorial.

    //Eliminamos la escena del menú de niveles.

    delete levels_menu;

    //La variable level_num almacena el nivel seleccionado por el usuario, 0 en caso de que el usuario
    //desee cursar de nuevo el tutorial o puede también tomar el valor de -1, lo cual representa que
    //el usuario desea salir del juego y regresar al menú princiapl de la interfaz gráfica.

    //En caso de querer regresar al menú principal emitimos la señal exit_game() conectada al slot
    //exit_game() de la clase Widget.

    if (level_num == -1) emit exit_game();
    else {

        //En caso de que se seleccione un nivel, almacenamos su número en current_level, o 0 en caso
        //de ser el tutorial (esto último ya viene en concordancia con el valor de level_num).

        current_level = level_num;

        if (current_level == 0) {

            //En caso de solicitar el tutorial, simplemente lo cargamos de nuevo.

            level = new Level(path, two_players, current_level);
            connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
            game_gv->setScene(level);
        }
        else {

            //En caso de que se desee cursar un nivel, lo construimos y colocamos en el QGraphicsView.

            //Una observación importante es que como terrain_matrices es un std::array y no un array
            //de la forma [], para pasar la dirección de memoria de alguno de sus elemetnos hay que
            //utilizar &(terrain_matrices[i]), en lugar de simplemente (terrain_matrices + i).

            level = new Level(path, two_players, current_level, levels_waves[level_num - 1],
                              &rocks_num, &glues_num, &extra_life, (healths + (level_num - 1)),
                              &(terrain_matrices[level_num - 1]));
            connect(level, &Level::update_level_progress, this, &Game::update_level_progress);
            game_gv->setScene(level);
        }
    }
}

void Game::update_level_progress(short progress_type) {

    //Este slot es activado por la señal update_level_progress() de la clase Level, y su propósito
    //principal es realizar alguna combinación de las siguientes dos tareas: terminar el nivel o
    //guardar el estado del juego, dependiendo del valor de progress_type y de current_level.

    //La variable progress_type puede tomar los valores desde el 0 hasta el 3, donde el 0 significa
    //que el usuario ha perdido el nivel, el 1 que el usuario ha salido del nivel mediante el menú
    //de pausa, el 2 representa que el usuario ha terminado una oleada y por tanto debemos guardar
    //la partida pero no cerrarla, y el 3 representa que el usuario ha completado el nivel.

    if (progress_type == 3) {

        //En caso de que el usuario haya superado el nivel, primero eliminamos la escena.

        delete level;

        //Si se completó un nivel como tal, no el tutorial, debemos limpiar el terreno de todos
        //los objetos que pueda tener, restaurar la salud del objeto a 1000 y colocar el nivel
        //desde la oleada 1.

        if (current_level != 0) {

            //Como se mencionó antes, como terrain_matrices es un std::array y no un array de la forma [],
            //para pasar la dirección de memoria de alguno de sus elemetnos hay que utilizar
            //&(terrain_matrices[i]), en lugar de simplemente (terrain_matrices + i).

            //Recordemos que como del nivel tutorial no guardamos información, los indices de los arreglos
            //terrain_matrices, levels_waves y healths van desde 0 hasta 3, pero current_level toma valores
            //desde 0 hasta 4, donde 0 representa el tutorial, por lo cual debemos restar uno al valor de
            //current_level para poder acceder al nivel correspondiente dentro de los arreglos.

            clear_terrain(&(terrain_matrices[current_level - 1]));
            levels_waves[current_level - 1] = 1;
            healths[current_level - 1] = 1000;
        }

        //En caso de que sea el nivel tutorial, como en este se pueden conseguir objetos muy fácilmente,
        //cargamos el juego al último guardado (justo antes de comenzar el tutorial) para evitar que el
        //usuario pueda conseguir objetos y vidas extra a partir del tutorial, aunque esto de poder
        //conseguir objeto y vidas extra se puede hacer, y está diseñado para que pueda hacerse,
        //superando un nivel más sencillo, para poder llegar a un nivel más difícil preparado.

        else load_game();

        //Posteriormente desbloqueamos el siguiente nivel, o en caso de que este sea el último
        //colocamos la variable winner en true.

        //La segunda condición del and del siguiente condicional es necesaria, pues solo podemos
        //colcar en 1 la oleada de niveles boqueados, ya que en caso de que el usuario repita
        //un nivel que ya superó, podria perder el progreso del siguiente al colocar la
        //oleada de este último en 1.

        if ((current_level < 3) and (levels_waves[current_level] == 0)) levels_waves[current_level] = 1;
        else if (current_level == 3) winner = true;

        //Notemos que aún no hemos guardado la partida, esto es porque aún falta que el usuario participe
        //del minijuego, para obtener recompensas extra, por lo cual lo construimos y
        //añadimos al QGraphicsView.

        minigame = new Minigame(two_players, &rocks_num, &glues_num, &extra_life);
        connect(minigame, &Minigame::minigame_finished, this, &Game::minigame_finished);
        game_gv->setScene(minigame);
    }
    else if (progress_type == 2) {

        //Si progress_type posee el valor de 2, significa que simplemente debemos guardar la partida pues se
        //acaba de superar una oleada, por lo cual aumentamos la oleada del nivel actual y guardamos el
        //juego mediante le método save_game.

        //Es importante notar que se poseen dos lugares donde se encutra la información del terreno,
        //dentro del obejto terrain de la clase Terrain dentro del objeto level, y en terrain_matrices,
        //por lo cual es importante mantener ambas actualizados, sin embargo, terrain_matrices, que es
        //de donde se toma la información para guardar la partida, es actualizada desde dentro de la
        //clase Level justo antes de emitir la señal update_level_progress(2), por lo cual aquí
        //simplemente basta aumentar la oleada y llamar el método save_game().

        levels_waves[current_level - 1]++;
        save_game();
    }
    else {

        //Si el usuario fue derrotado o salio del nivel por medio del menú de pausa,
        //primero eliminamos la escena.

        delete level;

        //Cargamos el juego al último guardado.

        load_game();

        //En caso de que el usuario haya sido derrotado, le ayudamos un poco en caso de que la vida de
        //la base haya quedado muy baja en el último guardado, haciendo que cuando vuevla a retomar el
        //nivel la vida de la base sea el máximo entre la vida que tenía y 500.

        if ((progress_type == 0) and (healths[current_level - 1] < 500)) {
            healths[current_level - 1] = 500;
            save_game();
        }

        //Finalmente volvemos al menú de niveles.

        levels_menu = new LevelsMenu(two_players, rocks_num, glues_num, extra_life, levels_waves, winner);
        connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
        game_gv->setScene(levels_menu);
    }
}

void Game::minigame_finished() {

    //Este slot es activado por la señal minigame_finished() de la clase Minigame, y su propósito es
    //guardar la partida con las recompensas obtenidas en el minijuego y retornar al menú de niveles.

    delete minigame;
    save_game();

    levels_menu = new LevelsMenu(two_players, rocks_num, glues_num, extra_life, levels_waves, winner);
    connect(levels_menu, &LevelsMenu::level_selected, this, &Game::level_selceted);
    game_gv->setScene(levels_menu);
}
