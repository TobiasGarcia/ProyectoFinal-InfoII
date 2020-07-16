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
    QList<Enemy*> enemies;
    QTimer *freez_timer;

    QGraphicsRectItem *health_bar;
    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_terrain();
    void display_hud();
    void hit_all_enemies();
    void make_connections(Enemy *enemy);
    void set_freez(bool freez);

public:
    Level();
    ~Level();
    Terrain *terrain;
public slots:
    void give_power(short power_type);
    void remove_enemy(short list_index);
    void defrost();

signals:
    void update_index(short removed_index);

};

#endif // LEVEL_H
