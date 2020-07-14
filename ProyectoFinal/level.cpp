#include "level.h"
#include "QDebug"

Level::Level() {

    srand(time(nullptr));

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.

    terrain = new Terrain;
    display_terrain();

    //Se mide en porcentaje, pero como 100.0, con una cifra decimal.
    initial_health = 1000;
    display_hud();

    base = new Base(health_bar);
    addItem(base);
    //addItem(base->center);

    carlos = new Enemy(9, 3, 1, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Chamaleon(-1, 6, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Mole(9, 4, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Snail(0, 13, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Porcupine(9, 12, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Owl(-1, 6, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    carlos = new Vulture(9, 4, this, terrain);
    addItem(carlos);
    connect(carlos, &Enemy::first_bite, base, &Base::bitten);
    connect(carlos->bite_timer, &QTimer::timeout, base, &Base::bitten);

    terrain->tiles[4][9] = 1;
    terrain->tiles[3][3] = 1;
    terrain->tiles[4][3] = 1;
    terrain->tiles[5][3] = 1;

//    carlos = new Enemy(439, 270, terrain);
//    addItem(carlos);

//    carlos = new Enemy(330, 161, terrain);
//    addItem(carlos);

    players = new Players(330, 330, 450, 330); //Colocarlos en múltiplos de 15.
    connect(players, &Players::add_fire_ball, this, &Level::add_fire_ball);

    players->setFlag(QGraphicsItem::ItemIsFocusable);
    players->setFocus();

    addItem(players->multi);
    addItem(players);
}

Level::~Level() {
    //delete carlos;
    delete players;
    delete terrain;
    delete health_bar;
    delete base;
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
}

void Level::display_hud() {

    QGraphicsRectItem *rect = new QGraphicsRectItem(221, 544, 337, 51);
    rect->setBrush(QColor(86, 86, 86));
    rect->setZValue(3);
    addItem(rect);

    health_bar = new QGraphicsRectItem(221, 544, 337*(initial_health/1000.0), 51);
    health_bar->setBrush(QColor(54, 104, 195));
    health_bar->setPen(QColor(54, 104, 195));
    health_bar->setZValue(3);
    addItem(health_bar);

    QGraphicsPixmapItem *pix_map = new QGraphicsPixmapItem;
    pix_map->setPixmap(QPixmap(":/images/resources/images/hud.png"));
    pix_map->setPos(0, 540);
    pix_map->setZValue(3);
    addItem(pix_map);

}

void Level::add_fire_ball(short x, short y) {
    fire_ball = new FireBall(x, y);
    addItem(fire_ball);
    fire_ball->test_collisions();
}





















































































