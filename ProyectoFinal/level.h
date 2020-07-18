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

    //La variable tamplate_on[] servirá para determinar cuando colocar la plantilla
    //para poder colocar las rocas o los fluidos. Cuando un jugador presiona la tecla
    //para colocar una roca por ejemplo, se le suma uno a la primera posición, de esta
    //forma es sólo cuando el valor almacenado en la primera posición es cero cuando
    //se debe retirar la plantialla, de otro modo es porque hay algún jugador que
    //la necesita.

    short initial_health, rock_index, fluid_index, template_on[2];
    PowerUp *power_up;
    QList<Enemy*> enemies;
    QGraphicsPixmapItem *rock_powers, *fluid_powers, *power_template;
    QTimer *freez_timer;
    QList<QGraphicsItem*> rock_ghost_collisions;

    //La variable ghost_rock es utilizada para revisar si la zona donde se
    //desea colocar una roca está disponible, o hay algún enemigo ocupándola.

    QGraphicsRectItem *health_bar, *ghost_rock;
    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_terrain();
    void display_hud();
    void hit_all_enemies();
    void make_connections(Enemy *enemy);
    void set_freez(bool freez);
    void initialize_templates();
    void add_rock(short i, short j);
    void add_fluid(short i, short j);

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
