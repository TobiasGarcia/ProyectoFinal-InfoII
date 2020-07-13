#ifndef SPECIAL_ENEMIES_H
#define SPECIAL_ENEMIES_H

#include <enemy.h>

class Snail: public Enemy {
private:
    TerrainObject *fluid;

    void add_fluid();
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Snail(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain) :
        Enemy(i, j, _type, _level, _terrain) {};
};

class Porcupine: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Porcupine(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain) :
        Enemy(i, j, _type, _level, _terrain) {};
};

class Owl: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Owl(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain);
};

//La barra de vida lo delataría, por lo cual es invulnerable
//cuando está invisible.

//Además, si choca contra una pared, el camaleón se transporta
//a otra zona circundante y se hace invisible de nuevo.

class Chamaleon: public Enemy {
private:
    bool visible;
    QTimer *camouflage_timer;

    void rock_collision();
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Chamaleon(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain);;

public slots:
    void camouflage();
};

#endif // SPECIAL_ENEMIES_H
