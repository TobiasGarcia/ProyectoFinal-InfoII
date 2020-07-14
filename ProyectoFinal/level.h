#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <players.h>
#include <terrain.h>
#include <terrainobject.h>
#include <enemy.h>
#include <fireball.h>
#include <base.h>
#include <powerup.h>
#include <special_enemies.h>

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    Base *base;
    FireBall *fire_ball;
    Enemy *carlos;
    Players *players;
    short initial_health;
    PowerUp *power_up;

    QGraphicsRectItem *health_bar;
    //void keyPressEvent(QKeyEvent *event);

public:
    Level();
    ~Level();
    Terrain *terrain;

    void display_terrain();
    void display_hud();
public slots:
    void add_fire_ball(short x, short y);
};

#endif // LEVEL_H
