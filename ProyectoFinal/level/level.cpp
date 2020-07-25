#include "level.h"
#include "QDebug"

void Level::keyPressEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    else if (event->key() == Qt::Key_Up) player1->move_dir[0] = true;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = true;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = true;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = true;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = true;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = true;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = true;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = true;

    else if ((event->key() == Qt::Key_P) and pause) emit update_level_progress(1);

    //Si se presiona la tecla Esc, debemos deciride cuando podemos entrar al menú de pausa:
    //si no estamos en el menú de pausa y no hay información en la pantalla, es decir,
    //information->scene() es nullptr, entramos al menú; si no estamos en el menú de
    //pausa pero sí hay información en la pantalla, es decir, information->scene()
    //no es nulo, NO entramos al menú de pausa; y si estamos en pausa siempre
    //deberemos poder salir de esta.

    else if ((event->key() == Qt::Key_Escape) and (pause or (information->scene() == nullptr))) {
        if (pause) removeItem(information);
        else information->display_message(389, 182, "\t\t     PAUSA\n\n"
                                                    "Presiona Esc de nuevo para volver al juego.\n"
                                                    "Presiona la tecla P para volver al menú\n"
                                                    "de niveles.");
        set_global_freez(!pause);
        pause = !pause;
    }

    //Si los jugadores están quietos, igual debemos actualizar su arreglo move_dir,
    //esto es para que si los jugadores dejan unidas algunas teclas y se retira
    //el freez, el juego si reaccione correctamente; por otra parte, debemos
    //retornar aquí en caso de que los jugadores estén en freez, pues
    //no podemos permitir que disparen fire balls o coloquen objetos
    //en el terreno.

    //Notemos que basta preguntar sólo al primer jugador pues él está congelado
    //si y solo si el segundo también.

    else if (player1->freez) return;

    //Qt::Key_Return es el enter cercano a las flechas, Qt::Key_Enter es el del Numeric Keypad.

    else if (!player1->delay_timer->isActive() and (event->key() == Qt::Key_Return)) add_fire_ball(player1->x(), player1->y());
    else if (two_players and !player2->delay_timer->isActive() and (event->key() == Qt::Key_Space)) add_fire_ball(player2->x(), player2->y());

    else if ((event->key() == Qt::Key_Backspace) or (two_players and (event->key() == Qt::Key_V))) {
        if (template_on == 0) addItem(power_template);
        template_on++;
    }
}

void Level::keyReleaseEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = false;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = false;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = false;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = false;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = false;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = false;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = false;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = false;

    //El propósito del siguiente condicional es análogo al que se encuentra en keyPressEvent().

    else if (player1->freez) return;

    else if (event->key() == Qt::Key_Backspace) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player1->y()/60, player1->x()/60);
    }
    else if (two_players and (event->key() == Qt::Key_V)) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player2->y()/60, player2->x()/60);
    }
    else if (event->key() == Qt::Key_Shift) add_fluid(player1->y()/60, player1->x()/60);
    else if (two_players and (event->key() == Qt::Key_B)) add_fluid(player2->y()/60, player2->x()/60);
}

Level::Level(std::string path, bool _two_players, short level_num, short initial_wave, short *_rocks_num,
             short *_fluids_num, bool *_extra_life, short *health, std::array<std::string, 9> *_terrain_matrix) :

             two_players(_two_players), extra_life(_extra_life), rocks_num(_rocks_num),
             fluids_num(_fluids_num), terrain_matrix(_terrain_matrix) {

    //(*wave) = 2;
    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.
    setBackgroundBrush(QBrush(QPixmap(":/textures/resources/images/floor_texture.png")));

    if (!get_level_script(path, level_num, initial_wave)) qDebug() << "No se abrió el archivo >:(";
    next = true;

    enemie = nullptr;

    gameover = false;

    active_timers[0] = false;
    active_timers[1] = false;
    active_timers[2] = false;

    instructions_timer = new QTimer;
    connect(instructions_timer, &QTimer::timeout, this, &Level::next_instruction);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Level::finish_delay);

    black_screen = new BlackScreen;
    connect(black_screen, &BlackScreen::finish, this, &Level::finish_level);
    addItem(black_screen);

    information = new Information(this);
//    information->display_message(390, 60, QString("GALAXY LACTERS"));

    terrain = new Terrain(this, terrain_matrix);
    while ((terrain->rocks_num + (*rocks_num)) > 15) (*rocks_num)--;

    display_terrain();

    initialize_template();

    //Un poco más grande para evitar hacer contacto con los enemigos
    //cuando sea colocada.
    ghost_rock = new QGraphicsRectItem(-10, -10, 80, 80);

    freez_timer = new QTimer;
    freez_timer->setSingleShot(true);
    connect(freez_timer, &QTimer::timeout, this, &Level::defrost);

    display_hud(*health);//Salud inicial de la base.

    lifebuoy = nullptr;
    if ((*extra_life)) {
        lifebuoy = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/lifebuoy_small.png"));
        lifebuoy->setOffset(-30, -30);
        lifebuoy->setPos(390, 270);
        lifebuoy->setZValue(3);
        addItem(lifebuoy);
    }

    base = new Base(health_bar, health, extra_life, lifebuoy);//Salud inicial de la base.
    connect(base, &Base::no_health, this, &Level::no_health);
    addItem(base);

////    enemie = new Snail(-1, 3, this, terrain, 0);
////    make_connections(enemie);
////    enemies.append(enemie);
////    addItem(enemie);

////    enemie = new Snail(2, -1, this, terrain, 1);
////    make_connections(enemie);
////    enemies.append(enemie);
////    addItem(enemie);

////    enemie = new Snail(9, 4, this, terrain, 2);
////    make_connections(enemie);
////    enemies.append(enemie);
////    addItem(enemie);

////    enemie = new Snail(5, 13, this, terrain, 3);
////    make_connections(enemie);
////    enemies.append(enemie);
////    addItem(enemie);

////    carlos = new Enemy(9, 3, 1, this, terrain, 1);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Chamaleon(-1, 6, this, terrain, 1);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Mole(9, 4, this, terrain, 2);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Snail(0, 13, this, terrain, 3);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Porcupine(9, 12, this, terrain, 4);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Owl(-1, 6, this, terrain, 5);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    carlos = new Vulture(9, 4, this, terrain, 6);
////    make_connections(carlos);
////    enemies.append(carlos);
////    addItem(carlos);

////    terrain->tiles[4][9] = new TerrainObject(540, 240, 1);
////    terrain->tiles[3][3] = new TerrainObject(180, 180, 1);
////    terrain->tiles[4][3] = new TerrainObject(180, 240, 1);
////    terrain->tiles[5][3] = new TerrainObject(180, 300, 1);

////    power_up = new PowerUp(rand()%4, 4);
////    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
////    addItem(power_up);

    power_up = nullptr;

    player1 = new Player(5, 5, true);
    addItem(player1);

    if (two_players) {
        player2 =  new Player(5, 7, true, false);
        addItem(player2);
    }

    pause = false;
    black_screen->change_opacity(false);
    instructions_timer->start(500);
    active_timers[0] = true;
}

Level::~Level() {
    delete information;
    delete player1;
    if (two_players) delete player2;
    delete terrain;
    delete health_bar;
    delete freez_timer;
    delete[] rock_powers;
    delete[] fluid_powers;
    delete power_template;
    delete ghost_rock;
    delete instructions_timer;
    delete delay_timer;
    delete black_screen;
    if (lifebuoy != nullptr) delete lifebuoy;
    delete base;
    if (power_up != nullptr) delete power_up;
}

void Level::give_power(short power_type) {

    power_up = nullptr;

    if (power_type == 0) hit_all_enemies();
    else if (power_type == 1) {
        set_enemies_freez(true);
        freez_timer->start(5000);
        active_timers[2] = true;
    }
    else if (power_type == 2) base->increase_health(200);
    else if (power_type == 3) {
        while ((*fluids_num) < 4) {
            (*fluids_num)++;
            addItem(fluid_powers + (*fluids_num) - 1);
        }
    }
    else {
        while (((*rocks_num) < 4) and ((terrain->rocks_num + (*rocks_num)) < 15)) {
            (*rocks_num)++;
            addItem(rock_powers + (*rocks_num) - 1);
        }
    }
}

void Level::remove_enemy(short list_index) {
    enemies.removeAt(list_index);
    emit update_index(list_index);
}

void Level::defrost() {
    set_enemies_freez(false);
    active_timers[2] = false;
}

void Level::add_enemie(short type) {

    short i, j, side = rand()%4;

    if (side%2) {
        i = rand()%9;
        j = 7*side - 8;
    }
    else {
        i = 5*side - 1;
        j = rand()%13;
    }

    //La memoria almacenada por enemie se libera cuando se le baja toda la vida.

    if (type == 8) {
        enemie = new Vulture(this, terrain, enemies.size());
        connect(enemie, &Enemy::vulture_hit, base, &Base::vulture_hit);
    }
    else if (type == 7) enemie = new Mole(this, terrain, enemies.size());
    else if (type == 6) enemie = new Chamaleon(i, j, this, terrain, enemies.size());
    else if (type == 5) enemie = new Owl(i, j, this, terrain, enemies.size());
    else if (type == 4) enemie = new Porcupine(i, j, this, terrain, enemies.size());
    else if (type == 3) enemie = new Snail(i, j, this, terrain, enemies.size());
    else enemie = new Enemy(i, j, type, this, terrain, enemies.size());
    make_connections(enemie);
    enemies.append(enemie);
    addItem(enemie);
}

void Level::add_power_up() {

    //El rand()%3 es para que sea más probable que salga true que false.

    if (enemies.size() > 3) power_up = new PowerUp(rand()%4, rand()%2);
    else if (base->get_health() < 500) power_up = new PowerUp(rand()%4, 2);
    else if (rand()%3 and (terrain->rocks_num < 15)) power_up = new PowerUp(rand()%4, 4);
    else power_up = new PowerUp(rand()%4, 3);

    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
    addItem(power_up);
}

void Level::next_instruction() {

    if (pause) return;

    instruction = script.front();

    //qDebug() << QString::fromUtf8(instruction.c_str());
    if (instruction[0] == 'W') {

        //El número de la oleada se aumenta cuando se guarda la partida
        //y no es la oleada final.

        information->display_message(389, 194, "Oleada " + QString(instruction[1]));
        information->set_display_time(3000);

        delay_timer->start(3000);
        active_timers[1] = true;

        instructions_timer->stop();
        active_timers[0] = false;
    }
    else if (instruction[0] == 'S') {
        //qDebug() << "STOP" << std::stoi(instruction.substr(1));
        delay_timer->start(std::stoi(instruction.substr(1)));
        active_timers[1] = true;

        instructions_timer->stop();
        active_timers[0] = false;
    }
    else if (instruction[0] == 'E') {
        if (next) {
            max_enemies = instruction.size() - 1;
            enemie_count = 0;
            next = false;
        }

        add_enemie(short(instruction[enemie_count + 1]) - 48);
        enemie_count++;

        if (max_enemies == enemie_count) next = true;
    }
    else if (instruction[0] == 'P') add_power_up();
    else if (instruction[0] == 'F') {
        if (next) {
            next = false;
            qDebug() << "Finish";
        }
        else if (enemies.isEmpty()) {
            next = true;

            //if (instruction[1] == '1') terrain->clean_fluid();
            if (instruction[1] == '0') {
                terrain->update_terrain_matrix(terrain_matrix);
                emit update_level_progress(2);
                information->display_message(389, 194, "¡Fin de la Oleada!");
                information->set_display_time(3000);
            }
            else information->display_message(389, 194, "Nivel Superado");

            delay_timer->start(3000);
            active_timers[1] = true;

            instructions_timer->stop();
            active_timers[0] = false;
        }
    }
    else {
        black_screen->change_opacity(true);
        instructions_timer->stop();
        active_timers[0] = false;
    }

    if (next) script.pop();
}

void Level::finish_delay() {

    if (gameover) black_screen->change_opacity(true);
    else {
        instructions_timer->start(500);
        active_timers[0] = true;
        active_timers[1] = false;
    }
}

void Level::finish_level() {
    emit update_level_progress((gameover?0:3));
}

void Level::no_health() {

    gameover = true;
    set_global_freez(true);
    information->display_message(389, 194, "La base ha sido destruida");

    //El timer delay_timer activará la pantalla negra, la cual posteriormente
    //activará el slot de finish_level, el cual emitirá la señal de
    //update_level_progress con un valor de 0 pues gameover
    //está en true.

    delay_timer->start(3000);
    active_timers[1] = true;
}

void Level::display_terrain() {

    for (short i = 0; i < 9; i++) for (short j = 0; j < 13; j++) {
        if (terrain->tiles[i][j] != nullptr) addItem(terrain->tiles[i][j]);
    }

//    terrain_object = new TerrainObject(60*9, 60*4, 1);
//    addItem(terrain_object);

//    terrain_object = new TerrainObject(60*3, 60*3, 1);
//    addItem(terrain_object);

//    terrain_object = new TerrainObject(60*3, 60*4, 1);
//    addItem(terrain_object);

//    terrain_object = new TerrainObject(60*3, 60*5, 1);
//    addItem(terrain_object);

//    QGraphicsLineItem *line;
//    for (short i = 0; i < 9; i++) {
//        line = new QGraphicsLineItem(0, 60*(i + 1) - 1, 779, 60*(i + 1) - 1);
//        addItem(line);
//    }
//    for (short j = 0; j < 12; j++) {
//        line = new QGraphicsLineItem(60*(j + 1) - 1, 0, 60*(j + 1) - 1, 599);
//        addItem(line);
//    }

//    QGraphicsRectItem *rect = new QGraphicsRectItem(QRect(254, 134, 265, 265));
//    addItem(rect);

//    QGraphicsEllipseItem *elli =  new QGraphicsEllipseItem(QRectF(60, 60, 659, 419));
//    addItem(elli);

//    QGraphicsEllipseItem *elli =  new QGraphicsEllipseItem(QRectF(389 - 20, 269 - 20, 40, 40));
//    addItem(elli);
}

void Level::display_hud(short initial_health) {

    QGraphicsRectItem *rect = new QGraphicsRectItem(221, 544, 337, 51);
    rect->setBrush(QColor(86, 86, 86));
    rect->setZValue(5);
    addItem(rect);

    health_bar = new QGraphicsRectItem(221, 544, 337*(initial_health/1000.0), 51);
    health_bar->setBrush(QColor(54, 104, 195));
    health_bar->setPen(QColor(54, 104, 195));
    health_bar->setZValue(5);
    addItem(health_bar);

    QGraphicsPixmapItem *pix_map = new QGraphicsPixmapItem;
    pix_map->setPixmap(QPixmap(":/images/resources/images/hud.png"));
    pix_map->setPos(0, 540);
    pix_map->setZValue(5);
    addItem(pix_map);

    rock_powers = new QGraphicsPixmapItem[4];
    for (short i = 0; i < 4; i++) {
        rock_powers[i].setPixmap(QPixmap(":/power_ups/resources/images/power_ups/rock_power.png"));
        rock_powers[i].setPos(20 + 50*i, 555);
        rock_powers[i].setZValue(5);
        if (i < (*rocks_num)) addItem(rock_powers + i);
    }

    fluid_powers = new QGraphicsPixmapItem[4];
    for (short i = 0; i < 4; i++) {
        fluid_powers[i].setPixmap(QPixmap(":/power_ups/resources/images/power_ups/fluid_power.png"));
        fluid_powers[i].setPos(730 - 50*i, 555);
        fluid_powers[i].setZValue(5);
        if (i < (*fluids_num)) addItem(fluid_powers + i);
    }
}

void Level::hit_all_enemies() {

    short index = 0, size = enemies.size();
    while (index < size) {

////        El topo coloca un agujero antes de aparecer, por lo cual tarda un poco
////        en entrar al campo visual y por tanto no se le deberá aplicar ningún efecto
////        de los power ups hasta que entre, no obstante, como él ya está en la lista de
////        enemigos, utilizamos el condicional para que el power up no le afecte a él
////        en caso de que aún no haya aparecido.

        if (enemies.at(index)->get_type() == 7) {
            Mole *mole = dynamic_cast<Mole*>(enemies.at(index));
            if (mole->dig_timer->isActive()) {
                index++;
                continue;
            }
        }

        enemies.at(index)->reduces_health(300);
        if (size == enemies.size()) index++;
        else size--;
    }
}

void Level::make_connections(Enemy *enemy) {
    connect(enemy, &Enemy::first_bite, base, &Base::bitten);
    connect(enemy->bite_timer, &QTimer::timeout, base, &Base::bitten);
    connect(enemy, &Enemy::remove_enemy, this, &Level::remove_enemy);
    connect(this, &Level::update_index, enemy, &Enemy::update_index);
}

void Level::set_enemies_freez(bool freez) {

    base->set_vulnerable(!freez);
    for (short i = 0; i < enemies.size(); i++) {

//        if (enemies.at(i)->get_type() == 7) {
//            Mole *mole = dynamic_cast<Mole*>(enemies.at(i));
//            if (mole->dig_timer->isActive()) continue;
//        }

        enemies.at(i)->set_freez(freez);
    }
}

void Level::initialize_template() {

    template_on = 0;

    power_template = new QGraphicsPixmapItem;
    power_template->setPixmap(QPixmap(":/interface/resources/images/interface/available.png"));
    power_template->setPos(180, 120);
    power_template->setOpacity(0.6);
    power_template->setZValue(0);
}

void Level::add_rock(short i, short j) {

    if ((*rocks_num) == 0) return;

    //Verificamos que sea una de las baldosas donde se pueden colocar
    //las rocas.

    bool border;
    if (((i == 2) or (i == 6)) and (3 < j) and (j < 9)) border = true;
    else if (((j == 3) or (j == 9)) and (2 < i) and (i < 6)) border = true;
    else border = false;

    if (!border) return;
    else {

        if ((terrain->tiles[i][j] != nullptr) and (terrain->tiles[i][j]->get_type() == 1)) return;

        QGraphicsItem *item;

        //Sorprendentemente no hace falta agregarlo a la escena para determinar
        //las colisiones.

        ghost_rock->setPos(60*j, 60*i);
        rock_ghost_collisions = collidingItems(ghost_rock, Qt::IntersectsItemBoundingRect);

        for (short i = 0; i < rock_ghost_collisions.size(); i++) {
            item = rock_ghost_collisions[i];

            //Solo hace falta revisar a estos enemigos ya que el resto no supone un obstáculo
            //para colocar una roca.

            if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail))
             or (typeid(*item) == typeid(Chamaleon))) {
                return;
            }
        }

        //Si se llega hasta aquí es porque no hay enemigos y se puede colocar
        //la roca.

        if (terrain->tiles[i][j] != nullptr) {
            removeItem(terrain->tiles[i][j]);
            delete terrain->tiles[i][j];
        }

        terrain->tiles[i][j] = new TerrainObject(i, j, 1);
        addItem(terrain->tiles[i][j]);

        removeItem(rock_powers + (*rocks_num) - 1);
        (*rocks_num)--;

        terrain->rocks_num++;
    }
}

void Level::add_fluid(short i, short j) {

    if ((*fluids_num) == 0) return;
    else if ((i == 4) and (j == 6)) return;
    else if (terrain->tiles[i][j] == nullptr) {
        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(fluid_powers + (*fluids_num) - 1);
        (*fluids_num)--;
    }
    else if (terrain->tiles[i][j]->get_type() == 2) {
        removeItem(terrain->tiles[i][j]);
        delete terrain->tiles[i][j];

        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(fluid_powers + (*fluids_num) - 1);
        (*fluids_num)--;
    }

    //Simplemente salimo si hay un fluido colocado por los jugadores
    //o una roca.

}

void Level::set_global_freez(bool freez) {

    //Permite detener el juego justo donde está, para luego poder
    //retormarlo justo como estaba. A exceción de Information.

    set_enemies_freez(freez);
    if (power_up != nullptr) power_up->set_freez(freez);

    player1->freez = freez;
    if (two_players) player2->freez = freez;

    if (freez) {
        if (active_timers[0]) {
            remainings[0] = instructions_timer->remainingTime();
            instructions_timer->stop();
        }
        if (active_timers[1]) {
            remainings[1] = delay_timer->remainingTime();
            delay_timer->stop();
        }
        if (active_timers[2]) {
            remainings[2] = freez_timer->remainingTime();
            freez_timer->stop();
        }
    }
    else {
        if (active_timers[0]) {
            instructions_timer->start(remainings[0]);
            active_timers[0] = true;
        }
        if (active_timers[1]) {
            delay_timer->start(remainings[1]);
            active_timers[1] = true;
        }
        if (active_timers[2]) {
            freez_timer->start(remainings[2]);
            active_timers[2] = true;
        }
    }
}

bool Level::get_level_script(std::string path, short level_num, short initial_wave) {

    bool current_game = false;
    std::fstream file(path + "levels_scripts/" + (two_players?'1':'0') + "level" + char(level_num + 48) + ".txt", std::ios::in);
    if (file.is_open()) {
        while (getline(file, instruction)) {

            if (current_game) script.push(instruction);
            else if ((instruction[0] == 'W') and ((short(instruction[1]) - 48) == initial_wave)) {
                script.push(instruction);
                current_game = true;
            }
        }
        file.close();

        //Le bajamos uno para que cuando se comiencen a ejecutar las
        //instrucciones se compense.

        return true;
    }
    else return false;
}

void Level::add_fire_ball(short x, short y) {

    //La memoria de la fire ball se libera cuando ella sola se destruye.

    fire_ball = new FireBall(x, y);
    addItem(fire_ball);
    //Tiene que ser después de que se agregó a la escena,
    //por eso va como un método público.
    fire_ball->test_collisions();
}





















































































