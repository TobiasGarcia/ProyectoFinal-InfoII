#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QFontDatabase>
#include "player.h"
#include "level/terrain.h"
#include "level/terrainobject.h"
#include "level/enemy.h"
#include "fireball.h"
#include "level/base.h"
#include "level/powerup.h"
#include <QKeyEvent>
#include "information.h"
#include "level/special_enemies.h"
#include "blackscreen.h"
#include <fstream>
#include <queue>

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    BlackScreen *black_screen;
    Base *base;
    FireBall *fire_ball;
    Enemy *enemie;
    Player *player1, *player2;

    //La variable tamplate_on[] servirá para determinar cuando colocar la plantilla
    //para poder colocar las rocas o los fluidos. Cuando un jugador presiona la tecla
    //para colocar una roca por ejemplo, se le suma uno a la primera posición, de esta
    //forma es sólo cuando el valor almacenado en la primera posición es cero cuando
    //se debe retirar la plantialla, de otro modo es porque hay algún jugador que
    //la necesita.

    bool next, pause, two_players, power_up_bool, extra_life;
    Information *information;
    short *rocks_num, *fluids_num, *wave, template_on, enemie_count, max_enemies;
    PowerUp *power_up;
    QList<Enemy*> enemies;
    QGraphicsPixmapItem *rock_powers, *fluid_powers, *power_template;
    QTimer *freez_timer, *instructions_timer, *delay_timer;
    QList<QGraphicsItem*> rock_ghost_collisions;

    std::string instruction;
    std::queue<std::string> script;

    //La variable ghost_rock es utilizada para revisar si la zona donde se
    //desea colocar una roca está disponible, o hay algún enemigo ocupándola.

    QGraphicsRectItem *health_bar, *ghost_rock;
    void add_enemie(short type);
    void add_power_up();
    bool get_level_script(std::string path, short level_num);
    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_terrain();
    void display_hud(short initial_health);
    void hit_all_enemies();
    void make_connections(Enemy *enemy);
    void set_freez(bool freez);
    void initialize_template();
    void add_rock(short i, short j);
    void add_fluid(short i, short j);

public:
    Level(std::string path, bool _two_players, short level_num, short *_wave, short *_rocks_num,
          short *_fluids_num, bool *_extra_life, std::array<std::string, 9> *terrain_matrix);
    ~Level();
    Terrain *terrain;
public slots:
    void give_power(short power_type);
    void remove_enemy(short list_index);
    void defrost();
    void next_instruction();
    void finish_delay();

signals:
    void update_index(short removed_index);

};

#endif // LEVEL_H
