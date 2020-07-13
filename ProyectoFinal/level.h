#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <players.h>
#include <terrain.h>
#include <terrainobject.h>
#include <enemy.h>
#include <fireball.h>
#include <base.h>

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    Base *base;
    FireBall *fire_ball;
    Enemy *carlos;
    Terrain *terrain;
    Players *players;
    short initial_health;


    QGraphicsRectItem *health_bar;

public:
    Level();
    ~Level();
    void display_terrain();
    void display_hud();
public slots:
    void add_fire_ball(short x, short y);
};

#endif // LEVEL_H
