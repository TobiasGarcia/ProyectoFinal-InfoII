#include <special_enemies.h>
#include <QDebug>

void Snail::add_fluid() {
    fluid = new TerrainObject(x() - short(x())%60, y() - short(y())%60, 2);
    level->addItem(fluid);
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
            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
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

                terrain->tiles[short(terrain_object->y()/60)][short(terrain_object->x()/60)] = 0;

                level->removeItem(terrain_object);
                delete terrain_object;

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

Owl::Owl(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain) :
    Enemy(i, j, _type, _level, _terrain) {

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

Chamaleon::Chamaleon(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain) :
    Enemy(i, j, _type, _level, _terrain) {

    visible = true;
    camouflage_timer = new QTimer;
    connect(camouflage_timer, &QTimer::timeout, this, &Chamaleon::camouflage);
    camouflage_timer->start(3000);
}



























































