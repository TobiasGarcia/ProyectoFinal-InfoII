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

//--------------------------------------------------------------------
//--------Mencionar que la mayor parte de la documentación se encuentra dentro de los .cpp y no en los .h.
//------------------------------------------------------------------------------------------

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

    bool next, pause, two_players, *extra_life, active_timers[3], gameover, dialoguing, tutorial_level;

    //Active timeres tiene bool sobre si los timers están activos o no:
    //0: instructions_timer
    //1: delay_timer
    //2: freez_timer

    //Análogo para remainings.

    Information *information;
    short *rocks_num, *fluids_num, template_on, enemie_count, max_enemies, remainings[3], action;
    PowerUp *power_up;
    QList<Enemy*> enemies;
    QGraphicsPixmapItem *rock_powers, *fluid_powers, *power_template, *lifebuoy;
    QTimer *freez_timer, *instructions_timer, *delay_timer, *action_timer;
    QList<QGraphicsItem*> rock_ghost_collisions;

    std::string instruction, pre_dialog, text;
    std::array<std::string, 2> dialog;
    std::queue<std::string> script;
    std::queue<std::array<std::string, 2>> dialogs;
    std::array<std::string, 9> *terrain_matrix;

    //La variable ghost_rock es utilizada para revisar si la zona donde se
    //desea colocar una roca está disponible, o hay algún enemigo ocupándola.

    QGraphicsRectItem *health_bar, *ghost_rock;
    void add_enemie(short type);
    void add_power_up();
    bool get_level_script(std::string path, short level_num, short initial_wave);
    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_terrain();
    void display_hud(short initial_health);
    void hit_all_enemies();
    void make_connections(Enemy *enemy);
    void set_enemies_freez(bool freez);
    void initialize_template();
    void add_rock(short i, short j);
    void add_fluid(short i, short j);
    void set_global_freez(bool freez);
    bool get_level_dialogs(std::string path, short level_num);
    void next_dialog();
    void set_up_tutorial_level();
    void remove_power_up();

public:
    Level(std::string path, bool _two_players, short level_num, short initial_wave, short *_rocks_num,
          short *_fluids_num, bool *_extra_life, short *health, std::array<std::string, 9> *terrain_matrix);
    ~Level();
    Terrain *terrain;
public slots:
    void give_power(short power_type);
    void remove_enemy(short list_index);
    void defrost();
    void next_instruction();
    void finish_delay();
    void finish_level();
    void no_health();
    void check_action();

signals:
    void update_index(short removed_index);

    //La variable progress puede tomar los siguientes valores:
    //0: el nivel se ha perdidio.
    //1: se ha salido del nivel por medio del menú de pausa.
    //2: se ha superado la oleada.
    //3: el nivel se ha completado.

    void update_level_progress(short progress_type);

};

#endif // LEVEL_H
