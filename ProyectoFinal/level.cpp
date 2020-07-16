#include "level.h"
#include "QDebug"

void Level::keyPressEvent(QKeyEvent *event) {

    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) player1->move_dir[0] = true;
    else if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) player1->move_dir[1] = true;
    else if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) player1->move_dir[2] = true;
    else if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) player1->move_dir[3] = true;

    else if ((event->key() == Qt::Key_W) and !event->isAutoRepeat()) player2->move_dir[0] = true;
    else if ((event->key() == Qt::Key_A) and !event->isAutoRepeat()) player2->move_dir[1] = true;
    else if ((event->key() == Qt::Key_S) and !event->isAutoRepeat()) player2->move_dir[2] = true;
    else if ((event->key() == Qt::Key_D) and !event->isAutoRepeat()) player2->move_dir[3] = true;

    //Qt::Key_Return es el enter cercano a las flechas, Qt::Key_Enter es el del Numeric Keypad.

    else if (!player1->delay_timer->isActive() and (event->key() == Qt::Key_Return) and !event->isAutoRepeat()) {
        add_fire_ball(player1->x(), player1->y());
    }
    else if (!player2->delay_timer->isActive() and (event->key() == Qt::Key_Space) and !event->isAutoRepeat()) {
        add_fire_ball(player2->x(), player2->y());
    }
}

void Level::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) player1->move_dir[0] = false;
    else if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) player1->move_dir[1] = false;
    else if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) player1->move_dir[2] = false;
    else if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) player1->move_dir[3] = false;

    else if ((event->key() == Qt::Key_W) and !event->isAutoRepeat()) player2->move_dir[0] = false;
    else if ((event->key() == Qt::Key_A) and !event->isAutoRepeat()) player2->move_dir[1] = false;
    else if ((event->key() == Qt::Key_S) and !event->isAutoRepeat()) player2->move_dir[2] = false;
    else if ((event->key() == Qt::Key_D) and !event->isAutoRepeat()) player2->move_dir[3] = false;
}

Level::Level() {

    srand(time(nullptr));

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.

    terrain = new Terrain;
    display_terrain();

    //Se mide en porcentaje, pero como 100.0, con una cifra decimal.
    initial_health = 1000;
    display_hud();

    freez_timer = new QTimer;
    freez_timer->setSingleShot(true);
    connect(freez_timer, &QTimer::timeout, this, &Level::defrost);

    base = new Base(health_bar);
    addItem(base);

    carlos = new Enemy(9, 3, 0, this, terrain, 0);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Chamaleon(-1, 6, this, terrain, 1);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Mole(9, 4, this, terrain, 2);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Snail(0, 13, this, terrain, 3);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Porcupine(9, 12, this, terrain, 4);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Owl(-1, 6, this, terrain, 5);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    carlos = new Vulture(9, 4, this, terrain, 6);
    make_connections(carlos);
    enemies.append(carlos);
    addItem(carlos);

    terrain->tiles[4][9] = 1;
    terrain->tiles[3][3] = 1;
    terrain->tiles[4][3] = 1;
    terrain->tiles[5][3] = 1;

    power_up = new PowerUp(1, 1);
    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
    addItem(power_up);

    player1 = new Player(5, 5);
    addItem(player1);

    player2 =  new Player(5, 7, false);
    addItem(player2);
}

Level::~Level() {
    //delete carlos;
    delete player1;
    delete player2;
    delete terrain;
    delete health_bar;
    delete freez_timer;
    //delete base;
    //delete power_up;
}

void Level::give_power(short power_type) {
    qDebug() << "Given power " << power_type;
    if (power_type == 0) hit_all_enemies();
    else if (power_type == 1) {
        set_freez(true);
        freez_timer->start(3000);
    }
}

void Level::remove_enemy(short list_index) {
    enemies.removeAt(list_index);
    emit update_index(list_index);
}

void Level::defrost() {
    set_freez(false);
}

void Level::display_terrain() {

    TerrainObject *terrain_object;

    for (short i = 0; i < 10; i++) for (short j = 0; j < 13; j++) {
        if (terrain->tiles[i][j]) {
            terrain_object = new TerrainObject(60*j, 60*i, terrain->tiles[i][j]);
            addItem(terrain_object);
        }
    }
    terrain_object = new TerrainObject(60*9, 60*4, 1);
    addItem(terrain_object);

    terrain_object = new TerrainObject(60*3, 60*3, 1);
    addItem(terrain_object);

    terrain_object = new TerrainObject(60*3, 60*4, 1);
    addItem(terrain_object);

    terrain_object = new TerrainObject(60*3, 60*5, 1);
    addItem(terrain_object);

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

void Level::display_hud() {

    QGraphicsRectItem *rect = new QGraphicsRectItem(221, 544, 337, 51);
    rect->setBrush(QColor(86, 86, 86));
    rect->setZValue(4);
    addItem(rect);

    health_bar = new QGraphicsRectItem(221, 544, 337*(initial_health/1000.0), 51);
    health_bar->setBrush(QColor(54, 104, 195));
    health_bar->setPen(QColor(54, 104, 195));
    health_bar->setZValue(4);
    addItem(health_bar);

    QGraphicsPixmapItem *pix_map = new QGraphicsPixmapItem;
    pix_map->setPixmap(QPixmap(":/images/resources/images/hud.png"));
    pix_map->setPos(0, 540);
    pix_map->setZValue(4);
    addItem(pix_map);

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

void Level::add_fire_ball(short x, short y) {
    fire_ball = new FireBall(x, y);
    addItem(fire_ball);
    //Tiene que ser después de que se agregó a la escena,
    //por eso va como un método público.
    fire_ball->test_collisions();
}





















































































