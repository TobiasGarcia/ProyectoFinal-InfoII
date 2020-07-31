#ifndef LEVEL_H
#define LEVEL_H

#include <queue>
#include <fstream>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QGraphicsScene>

#include "game/information.h"
#include "game/blackscreen.h"

#include "player/fireball.h"
#include "player/player.h"

#include "level/special_enemies.h"
#include "level/terrainobject.h"
#include "level/terrain.h"
#include "level/powerup.h"
#include "level/enemy.h"
#include "level/base.h"

//Esta clase modelará los niveles del juego.

class Level: public QGraphicsScene {

    Q_OBJECT

private:
    Base *base;
    Enemy *enemie;
    Terrain *terrain;
    PowerUp *power_up;
    FireBall *fire_ball;

    //Cuando agregamos un enemigo a la escena, lo agregamos también a la QList enemies, esto es para poder
    //utilizar el power up para golpear a todos los enemigos y el que es para congelarlos; utilzamos una
    //lista porque los enemigos pueden irse eliminando en cualquier orden, pero en lugar de std::list
    //preferimos QList pues la clase Enemy hereda a la clase QGraphicsPixmap, y las QList interactuan
    //más facilmente con los objetos propios de Qt.

    QList<Enemy*> enemies;
    Information *information;

    //Declaramos el objeto del jugador dos, pero sólo lo utilizamos sí efectivamente hay dos jugadores.

    Player *player1, *player2;
    BlackScreen *black_screen;

    QGraphicsRectItem *health_bar, *ghost_rock;
    QList<QGraphicsItem*> rock_ghost_collisions;
    QTimer *freez_timer, *instructions_timer, *delay_timer, *action_timer;
    QGraphicsPixmapItem *rock_powers, *glue_powers, *power_template, *lifebuoy;
    short *rocks_num, *glues_num, template_on, enemie_count, max_enemies, remainings[3], action;
    bool next, pause, two_players, *extra_life, active_timers[3], gameover, dialoguing, tutorial_level;

    std::queue<std::array<std::string, 2>> dialogs;
    std::array<std::string, 9> *terrain_matrix;
    std::array<std::string, 2> dialog;
    std::queue<std::string> script;
    std::string instruction, text;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    bool get_level_script();
    bool get_level_script(std::string path, short level_num, short initial_wave);
    bool get_level_dialogs(std::string path, short level_num);

    void display_terrain();
    void display_hud(short initial_health);

    void initialize_flow_variables(short level_num);
    void initialize_objects(short *health);
    void initialize_base(short *health);
    void initialize_template();
    void initialize_timers();

    void set_up_tutorial(short *(&health));
    void set_up_level(std::string path, short level_num, short initial_wave);

    void add_fire_ball(short x, short y);
    void add_rock(short i, short j);
    void add_glue(short i, short j);
    bool border(short i, short j);
    void add_enemie(short type);

    void make_connections(Enemy *enemy);

    void set_enemies_freez(bool freez);
    void set_global_freez(bool freez);

    void hit_all_enemies();

    void next_dialog();

    void start_level();
    void wait_for_user_action();

    void add_enemies(const std::string &instruction);
    void start_wave(short wave);
    void level_completed();
    void wait(short mili);
    void add_power_up();
    void finish_wave();

public:
    Level(std::string path, bool _two_players, short level_num, short initial_wave = 1,
          short *_rocks_num = nullptr, short *_glues_num = nullptr, bool *_extra_life = nullptr,
          short *health = nullptr, std::array<std::string, 9> *terrain_matrix  = nullptr);
    ~Level();

public slots:
    void remove_power_up();
    void give_power(short power_type);

    void remove_enemy(short list_index);

    void finish_delay();
    void defrost();

    void no_health();
    void finish_level();

    void next_instruction();
    void check_action();

signals:
    void update_index(short removed_index);

    //La variable progress_type de la siguiente señal puede tomar los siguientes valores:

    //0: el nivel se ha perdidio.
    //1: se ha salido del nivel por medio del menú de pausa.
    //2: se ha superado la oleada.
    //3: el nivel se ha completado.

    void update_level_progress(short progress_type);

};

#endif // LEVEL_H
