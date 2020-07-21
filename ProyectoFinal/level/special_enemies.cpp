#include "special_enemies.h"
#include <QDebug>

void Snail::add_fluid() {
    short i = y()/60, j = x()/60;

    //Es suficiente con verificar que no sea nulo pues este método no se llamará
    //sobre fluido de caracol y nunca estará sobre una roca.

    if (terrain->tiles[i][j] != nullptr) {
        level->removeItem(terrain->tiles[i][j]);
        delete terrain->tiles[i][j];
    }

    terrain->tiles[i][j] = new TerrainObject(i, j, 2);
    level->addItem(terrain->tiles[i][j]);
}

bool Snail::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    bool in_fluid = false;
    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {
            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);

            if ((terrain_object->get_type() == 1) and !rotated) {
                rock_collision();
                return true;
            }

            //No hay problema con el retorno porque para llegar hasta una roca
            //ya se debe haber puesto el fluido.

            else if (terrain_object->get_type() == 2) in_fluid = true;
        }
        else if (typeid(*item) == typeid(Base)) {
            move_timer->stop();

            //No se puede emitir la señal timeout manualmente,
            //por lo cual definimos una señal auxiliar para
            //enviar al comenzar el timer.
            emit first_bite();
            bite_timer->start(1000);

            //No hay problema con el retorno porque para llegar hasta la base
            //ya se debe haber puesto el fluido.

            return true;
        }
    }
    if (!in_fluid) add_fluid();
    return false;
}

bool Porcupine::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {

            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);
            if ((terrain_object->get_type() == 1) and !rotated) {

                terrain->tiles[short(terrain_object->y()/60)][short(terrain_object->x()/60)] = nullptr;

                level->removeItem(terrain_object);
                delete terrain_object;
                terrain->rocks_num--;

                emit remove_enemy(list_index);
                delete this;
                return true;
            }
            else if (terrain_object->get_type() == 2) speed = 1.4*speed_aux;
            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
        }
        else if (typeid(*item) == typeid(Base)) {
            move_timer->stop();

            //No se puede emitir la señal timeout manualmente,
            //por lo cual definimos una señal auxiliar para
            //enviar al comenzar el timer.
            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

Porcupine::Porcupine(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 4, _level, _terrain, _list_index) {

    targets.clear();
    targets.enqueue(QVector2D(389, 269));
    update_target();
}

bool Owl::collisions_handler(QList<QGraphicsItem *> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(Base)) {
            move_timer->stop();

            //No se puede emitir la señal timeout manualmente,
            //por lo cual definimos una señal auxiliar para
            //enviar al comenzar el timer.
            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

Owl::Owl(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 5, _level, _terrain, _list_index) {

    targets.clear();
    targets.enqueue(QVector2D(389, 269));
    update_target();
}

void Chamaleon::camouflage() {

    if (visible) {
        visible = false;
        level->removeItem(this);
    }
    else {
        visible = true;
        level->addItem(this);
    }
}

void Chamaleon::rock_collision() {

    visible = false;
    level->removeItem(this);

    short random_tile = rand()%16, tile[2];

    if (random_tile < 5) setPos(tiles2pixels(1, rand()%5 + 4).toPoint());
    else if (random_tile < 10) setPos(tiles2pixels(7, rand()%5 + 4).toPoint());
    else if (random_tile < 13) setPos(tiles2pixels(rand()%3 + 3, 2).toPoint());
    else setPos(tiles2pixels(rand()%3 + 3, 10).toPoint());

    targets.clear();
    recalculate_initial_tile(tile);
    set_targets(tile[0], tile[1]);
    update_target();

    camouflage_timer->start(3000);
}

bool Chamaleon::collisions_handler(QList<QGraphicsItem *> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    if (((240 <= x()) and (x() <= 540)) and ((180 <= y()) and (y() <= 360))) {
        if (camouflage_timer->isActive()) camouflage_timer->stop();
        if (!visible) {
            visible = true;
            level->addItem(this);
        }
    }

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {

            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);
            if ((terrain_object->get_type() == 1) and !rotated) {
                rock_collision();
                return true;
            }
            else if (terrain_object->get_type() == 2) speed = 1.4*speed_aux;
            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
        }
        else if (typeid(*item) == typeid(Base)) {
            move_timer->stop();

            //No se puede emitir la señal timeout manualmente,
            //por lo cual definimos una señal auxiliar para
            //enviar al comenzar el timer.
            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

Chamaleon::Chamaleon(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 6, _level, _terrain, _list_index) {

    visible = true;
    camouflage_timer = new QTimer;
    connect(camouflage_timer, &QTimer::timeout, this, &Chamaleon::camouflage);
    camouflage_timer->start(3000);
}

Mole::Mole(QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(0, -1, 7, _level, _terrain, _list_index) {

    move_timer->stop();

    short random_tile = rand()%6;

    if (random_tile < 3) tile_pixels = tiles2pixels(rand()%3 + 3, 4);
    else tile_pixels = tiles2pixels(rand()%3 + 3, 8);

    hole = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/bitten.png"));
    hole->setPos(tile_pixels[0] - 11, tile_pixels[1] - 11);
    level->addItem(hole);

    targets.clear();
    targets.enqueue(QVector2D(389, 269));
    rotated = true;

    dig_timer = new QTimer;
    dig_timer->setSingleShot(true);
    connect(dig_timer, &QTimer::timeout, this, &Mole::spawn);

    dig_timer->start(1000);
}

void Mole::spawn() {

    level->removeItem(hole);
    delete hole;

    setPos(tile_pixels[0], tile_pixels[1]);
    update_target();

    move_timer->start(50);
}

//void Vulture::set_targets(short i, short j) {

//    short initial_target;
//    QVector2D route[8] = {QVector2D(89, 209), QVector2D(89, 329), QVector2D(269, 509), QVector2D(509, 509),
//                                QVector2D(689, 329), QVector2D(689, 209), QVector2D(509, 29), QVector2D(269, 29)};

//    if ((j == -1) and (i < 4)) initial_target = 1;
//    else if (j == -1) initial_target = 2;

//    else if ((i == 9) and (j <= 6)) initial_target = 3;
//    else if (i == 9) initial_target = 4;

//    else if ((j == 13) and (i >= 4)) initial_target = 5;
//    else if (j == 13) initial_target = 6;

//    else if ((i == -1) and (j > 6)) initial_target = 7;
//    else initial_target = 0;

//    for (short k = 0; k < 16; k++) targets.enqueue(route[(k + initial_target)%8]);
//    targets.enqueue(QVector2D(389, 269));
//}

//bool Vulture::collisions_handler(QList<QGraphicsItem *> collisions) {

//    //Retornamos true si necesitamos dejar de ejecutar el slot move().

//    for (short i = 0; i < collisions.size(); i++) {
//        QGraphicsItem *item = collisions[i];
//        if (typeid(*item) == typeid(Base)) {
//            move_timer->stop();

//            //No se puede emitir la señal timeout manualmente,
//            //por lo cual definimos una señal auxiliar para
//            //enviar al comenzar el timer.

//            //EMITIR SEÑAL PARA EXPLOTAR LA BASE.

//            emit first_bite();
//            bite_timer->start(1000);
//            return true;
//        }
//    }
//    return false;
//}

double Vulture::radio() {
    return 1/sqrt(pow(cos(angle)/a, 2) + pow(sin(angle)/b, 2));
}

double Vulture::diff_radio() {
    return (0.5*a*b*(b*b - a*a))*(sin(2*angle)/pow(pow(b*cos(angle), 2) + pow(a*sin(angle), 2) , 1.5));
}

Vulture::Vulture(QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(0, -1, 8, _level, _terrain, _list_index) {

    a = 330; b = 210; c = sqrt(a*a + b*b);

    loop_num = 0;

    setPos(389, 269 - c);
    setRotation(-135);
    strike = false;
    straight_line = true;
    offset = 0;

    initial_angle = atan2(b*b/c, -a*a/c);
    angle = initial_angle;
}

void Vulture::move() {

    if (freez) return;

    if (straight_line) {
        setPos(x() - 0.1*spd, y() + 0.1*spd);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        if (sqrt(pow((389 - a*a/c) - x(), 2) + pow((269 - b*b/c) - y(), 2)) < 5) {
            straight_line = false;
            r = radio();
            setPos(389 + r*cos(angle), 269 - r*sin(angle) - offset);
            spd = 330;
        }
    }
    else {

        //Usamos la derivada implícita para obtener la pendiente en cualquier
        //punto de la elipse.

        setRotation(-90 - atan2(-b*b*(x() - 389), a*a*(269 - y() - offset))*180/M_PI);

        dr = diff_radio();
        angle += 0.06*spd/sqrt(dr*dr + r*r);
        //qDebug() << angle << "  " << 5*M_PI/2;
        if (abs(angle - 5*M_PI/2) < 0.1) {

            angle -= 2*M_PI;
            loop_num++;

            if (loop_num == 2) {
                a /= 2; b /= 2;
                offset = b;
                strike = true;
            }
        }

        r = radio();
        setPos(389 + r*cos(angle), 269 - r*sin(angle) - offset);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        if (strike) collisions_handler(collidingItems(Qt::IntersectsItemBoundingRect));
    }
}

bool Vulture::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    QGraphicsItem *item;
    for (short i = 0; i < collisions.size(); i++) {
        item = collisions[i];
        if (typeid(*item) == typeid(Base)) {

            move_timer->stop();
            qDebug() << "EXPLOUIIII!";
            return true;
        }
    }
    return false;
}

























































