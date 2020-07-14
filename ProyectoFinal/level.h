#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <player.h>
#include <terrain.h>
#include <terrainobject.h>
#include <enemy.h>
#include <fireball.h>
#include <base.h>
#include <powerup.h>
#include <QKeyEvent>
#include <special_enemies.h>

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    Base *base;
    FireBall *fire_ball;
    Enemy *carlos;
    Player *player1, *player2;
    short initial_health;
    PowerUp *power_up;

    QGraphicsRectItem *health_bar;
    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_terrain();
    void display_hud();

public:
    Level();
    ~Level();
    Terrain *terrain;

};

#endif // LEVEL_H
