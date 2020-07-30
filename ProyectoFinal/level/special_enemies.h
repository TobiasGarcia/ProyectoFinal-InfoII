#ifndef SPECIAL_ENEMIES_H
#define SPECIAL_ENEMIES_H

#include "level/enemy.h"

class Snail: public Enemy {
private:
    TerrainObject *glue;

    void add_glue();
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Snail(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
        Enemy(i, j, 3, _level, _terrain, _list_index) {};
};

class Porcupine: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Porcupine(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
};

class Owl: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);
public:
    Owl(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
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
    Chamaleon(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Chamaleon();

public slots:
    void camouflage();
};

class Mole: public Enemy {
private:
    QVector2D tile_pixels;
    QGraphicsPixmapItem *hole;
public:
    QTimer *dig_timer;
    Mole(QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Mole();
public slots:
    void spawn();
};

class Vulture: public Enemy {
private:
    short a, b, loop_num;
    bool straight_line, strike;
    double angle, initial_angle, r, dr, c, offset;

    double radio();
    double diff_radio();
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Vulture(QGraphicsScene *_level, Terrain *_terrain, short _list_index);
public slots:
    void move();
};

#endif // SPECIAL_ENEMIES_H
