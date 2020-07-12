#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <players.h>
#include <terrain.h>
#include <terrainobject.h>
#include <enemy.h>
#include <fireball.h>

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    FireBall *fire_ball;
    Enemy *carlos;
    Terrain *terrain;
    Players *players;
public:
    Level();
    ~Level();
    void display_terrain();
public slots:
    void add_fire_ball(short x, short y);
};

#endif // LEVEL_H
