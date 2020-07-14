#ifndef SPECIAL_ENEMIES_H
#define SPECIAL_ENEMIES_H

#include <enemy.h>

class Snail: public Enemy {
private:
    TerrainObject *fluid;

    void add_fluid();
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Snail(short i, short j, QGraphicsScene *_level, Terrain *_terrain) :
        Enemy(i, j, 3, _level, _terrain) {};
};

class Porcupine: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Porcupine(short i, short j, QGraphicsScene *_level, Terrain *_terrain);;
};

class Owl: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Owl(short i, short j, QGraphicsScene *_level, Terrain *_terrain);
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
    Chamaleon(short i, short j, QGraphicsScene *_level, Terrain *_terrain);;

public slots:
    void camouflage();
};

class Mole: public Enemy {
private:
    QTimer *dig_timer;
    QVector2D tile_pixels;
    QGraphicsPixmapItem *hole;
public:
    Mole(short i, short j, QGraphicsScene *_level, Terrain *_terrain);
public slots:
    void spawn();
};

class Vulture: public Enemy {
private:
    void set_targets(short i, short j);
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Vulture(short i, short j, QGraphicsScene *_level, Terrain *_terrain);
};

#endif // SPECIAL_ENEMIES_H
