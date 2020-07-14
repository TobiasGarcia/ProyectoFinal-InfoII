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

//    base = new Base(health_bar);
//    addItem(base);

//    carlos = new Enemy(9, 3, 0, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Chamaleon(-1, 6, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Mole(9, 4, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Snail(0, 13, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Porcupine(9, 12, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Owl(-1, 6, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

//    carlos = new Vulture(9, 4, this, terrain);
//    addItem(carlos);
//    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
//    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    terrain->tiles[4][9] = 1;
    terrain->tiles[3][3] = 1;
    terrain->tiles[4][3] = 1;
    terrain->tiles[5][3] = 1;

    power_up = new PowerUp(1, 0);
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
    //delete base;
    delete power_up;
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

void Level::add_fire_ball(short x, short y) {
    fire_ball = new FireBall(x, y);
    addItem(fire_ball);
    //Tiene que ser después de que se agregó a la escena,
    //por eso va como un método público.
    fire_ball->test_collisions();
}





















































































