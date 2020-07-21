#include "level.h"
#include "QDebug"

void Level::keyPressEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = true;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = true;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = true;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = true;

    else if (event->key() == Qt::Key_W) player2->move_dir[0] = true;
    else if (event->key() == Qt::Key_A) player2->move_dir[1] = true;
    else if (event->key() == Qt::Key_S) player2->move_dir[2] = true;
    else if (event->key() == Qt::Key_D) player2->move_dir[3] = true;

    //Qt::Key_Return es el enter cercano a las flechas, Qt::Key_Enter es el del Numeric Keypad.

    else if (!player1->delay_timer->isActive() and (event->key() == Qt::Key_Return)) add_fire_ball(player1->x(), player1->y());
    else if (!player2->delay_timer->isActive() and (event->key() == Qt::Key_Space)) add_fire_ball(player2->x(), player2->y());

    else if ((event->key() == Qt::Key_Backspace) or (event->key() == Qt::Key_V)) {
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

    else if (event->key() == Qt::Key_W) player2->move_dir[0] = false;
    else if (event->key() == Qt::Key_A) player2->move_dir[1] = false;
    else if (event->key() == Qt::Key_S) player2->move_dir[2] = false;
    else if (event->key() == Qt::Key_D) player2->move_dir[3] = false;

    else if (event->key() == Qt::Key_Backspace) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player1->y()/60, player1->x()/60);
    }
    else if (event->key() == Qt::Key_V) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player2->y()/60, player2->x()/60);
    }
    else if (event->key() == Qt::Key_Shift) add_fluid(player1->y()/60, player1->x()/60);
    else if (event->key() == Qt::Key_B) add_fluid(player2->y()/60, player2->x()/60);
}

Level::Level() {

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.
    setBackgroundBrush(QBrush(QPixmap(":/textures/resources/images/floor_texture.png")));

    if (!get_level_script()) qDebug() << "No se abrió el archivo >:(";
    pop = true;

    instructions_timer = new QTimer;
    connect(instructions_timer, &QTimer::timeout, this, &Level::next_instruction);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Level::finish_delay);


    information = new Information(this);
//    information->display_message(390, 60, QString("Estadisticas:\n"
//                                                  "\tHabilidad: 1000\n"
//                                                  "\tConcurrencia: 4200\n"
//                                                  "\tBelleza: Exquisita\n"
//                                                  "\n"
//                                                  "Nota: A+"));
//    information->setZValue(7);

    terrain = new Terrain(this);
    display_terrain();

    //Se mide en porcentaje, pero como 100.0, con una cifra decimal.
    rock_index = -1;
    fluid_index = -1;

    initialize_template();

    //Un poco más grande para evitar hacer contacto con los enemigos
    //cuando sea colocada.
    ghost_rock = new QGraphicsRectItem(-10, -10, 80, 80);

    freez_timer = new QTimer;
    freez_timer->setSingleShot(true);
    connect(freez_timer, &QTimer::timeout, this, &Level::defrost);

    display_hud(1000);//Salud inicial de la base.
    base = new Base(health_bar, 1000);//Salud inicial de la base.
    addItem(base);

//    carlos = new Enemy(9, 3, 1, this, terrain, 1);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Chamaleon(-1, 6, this, terrain, 1);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Mole(9, 4, this, terrain, 2);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Snail(0, 13, this, terrain, 3);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Porcupine(9, 12, this, terrain, 4);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Owl(-1, 6, this, terrain, 5);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    carlos = new Vulture(9, 4, this, terrain, 6);
//    make_connections(carlos);
//    enemies.append(carlos);
//    addItem(carlos);

//    terrain->tiles[4][9] = new TerrainObject(540, 240, 1);
//    terrain->tiles[3][3] = new TerrainObject(180, 180, 1);
//    terrain->tiles[4][3] = new TerrainObject(180, 240, 1);
//    terrain->tiles[5][3] = new TerrainObject(180, 300, 1);

//    power_up = new PowerUp(3, 3);
//    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
//    addItem(power_up);

    player1 = new Player(5, 5);
    addItem(player1);

    player2 =  new Player(5, 7, false);
    addItem(player2);

    instructions_timer->start(500);
}

Level::~Level() {
    //delete carlos;
    delete information;
    delete player1;
    delete player2;
    delete terrain;
    delete health_bar;
    delete freez_timer;
    delete[] rock_powers;
    delete[] fluid_powers;
    delete power_template;
    delete ghost_rock;
    delete instructions_timer;
    delete delay_timer;
    //delete base;
    //delete power_up;
}

void Level::give_power(short power_type) {

    if (power_type == 0) hit_all_enemies();
    else if (power_type == 1) {
        set_freez(true);
        freez_timer->start(3000);
    }
    else if (power_type == 2) base->increase_health(200);
    else if (power_type == 3) {
        while (fluid_index < 3) {
            fluid_index++;
            addItem(fluid_powers + fluid_index);
        }
    }
    else {
        while ((rock_index < 3) and ((terrain->rocks_num + (rock_index + 1)) < 15)) {
            rock_index++;
            addItem(rock_powers + rock_index);
        }
    }
}

void Level::remove_enemy(short list_index) {
    enemies.removeAt(list_index);
    emit update_index(list_index);
}

void Level::defrost() {
    set_freez(false);
}

void Level::add_enemie(short type) {

    short i, j, side = rand()%4;

    if (side%2) {
        i = rand()%8;
        j = 7*side - 8;
    }
    else {
        i = 5*side - 1;
        j = rand()%13;
    }

    if (type == 8) enemie = new Vulture(this, terrain, enemies.size());
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

    if (enemies.size() > 3) power_up = new PowerUp(rand()%4, rand()%2);
    else if (base->get_health() < 500) power_up = new PowerUp(rand()%4, 2);
    else if (rand()%2 and (terrain->rocks_num < 15)) power_up = new PowerUp(rand()%4, 4);
    else power_up = new PowerUp(rand()%4, 3);

    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
    addItem(power_up);
}

void Level::next_instruction() {

    instruction = script.front();

    qDebug() << QString::fromUtf8(instruction.c_str());
    if (instruction[0] == 'W') qDebug() << "Wave " << instruction[1];
    else if (instruction[0] == 'S') {
        qDebug() << "STOP" << std::stoi(instruction.substr(1));
        delay_timer->start(std::stoi(instruction.substr(1)));
        instructions_timer->stop();
    }
    else if (instruction[0] == 'E') {
        if (pop) {
            max_enemies = instruction.size() - 1;
            enemie_count = 0;
            pop = false;
        }

        add_enemie(short(instruction[enemie_count + 1]) - 48);
        enemie_count++;

        if (max_enemies == enemie_count) pop = true;
    }
    else if (instruction[0] == 'P') add_power_up();
    else {
        qDebug() << "Finish";
        instructions_timer->stop();
    }

    if (pop) script.pop();
}

void Level::finish_delay() {
    instructions_timer->start(500);
}

void Level::display_terrain() {

    for (short i = 0; i < 10; i++) for (short j = 0; j < 13; j++) {
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
        if (i <= rock_index) addItem(rock_powers + i);
    }

    fluid_powers = new QGraphicsPixmapItem[4];
    for (short i = 0; i < 4; i++) {
        fluid_powers[i].setPixmap(QPixmap(":/power_ups/resources/images/power_ups/fluid_power.png"));
        fluid_powers[i].setPos(730 - 50*i, 555);
        fluid_powers[i].setZValue(5);
        if (i <= fluid_index) addItem(fluid_powers + i);
    }
}

void Level::hit_all_enemies() {

    short index = 0, size = enemies.size();
    while (index < size) {

        //El topo coloca un agujero antes de aparecer, por lo cual tarda un poco
        //en entrar al campo visual y por tanto no se le deberá aplicar ningún efecto
        //de los power ups hasta que entre, no obstante, como él ya está en la lista de
        //enemigos, utilizamos el condicional para que el power up no le afecte a él
        //en caso de que aún no haya aparecido.

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

void Level::set_freez(bool freez) {

    base->set_vulnerable(!freez);
    for (short i = 0; i < enemies.size(); i++) {

        if (enemies.at(i)->get_type() == 7) {
            Mole *mole = dynamic_cast<Mole*>(enemies.at(i));
            if (mole->dig_timer->isActive()) continue;
        }

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

    if (rock_index == -1) return;

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

        removeItem(rock_powers + rock_index);
        rock_index--;

        terrain->rocks_num++;
    }
}

void Level::add_fluid(short i, short j) {

    if (fluid_index == -1) return;
    else if ((i == 4) and (j == 6)) return;
    else if (terrain->tiles[i][j] == nullptr) {
        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(fluid_powers + fluid_index);
        fluid_index--;
    }
    else if (terrain->tiles[i][j]->get_type() == 2) {
        removeItem(terrain->tiles[i][j]);
        delete terrain->tiles[i][j];

        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(fluid_powers + fluid_index);
        fluid_index--;
    }

    //Simplemente salimo si hay un fluido colocado por los jugadores
    //o una roca.

}

bool Level::get_level_script() {

    std::fstream file("../ProyectoFinal/data/levels_scripts/level1.txt", std::ios::in);
    if (file.is_open()) {
        while (getline(file, instruction)) script.push(instruction);
        file.close();
        return true;
    }
    else return false;
}

void Level::add_fire_ball(short x, short y) {
    fire_ball = new FireBall(x, y);
    addItem(fire_ball);
    //Tiene que ser después de que se agregó a la escena,
    //por eso va como un método público.
    fire_ball->test_collisions();
}





















































































