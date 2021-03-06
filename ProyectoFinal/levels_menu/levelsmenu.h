#ifndef LEVELSMENU_H
#define LEVELSMENU_H

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "levels_menu/needle.h"

#include "game/blackscreen.h"

#include "player/fireball.h"
#include "player/player.h"

//Esta clase modela el menú para que el jugador pueda escoger los niveles.

class LevelsMenu: public QGraphicsScene {

    Q_OBJECT

private:
    Needle *needle;
    Player *player1, *player2;
    BlackScreen *black_screen;

    short state, level_num, *levels_waves;
    bool two_players;

    void add_fire_ball(short x, short y);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void display_hud(short rocks_num, short glues_num, bool extra_life, bool winner);
    void display_powers(short rocks_num, short glues_num);
    void display_levels();

    void initialize_objects();

public:
    LevelsMenu(bool _two_players, short rocks_num, short glues_num, bool extra_life,
               short *_levels_waves, bool winner);
    ~LevelsMenu();

public slots:
    void needle_finish();
    void black_screen_finish();

signals:
    void level_selected(short level_num);
};

#endif // LEVELSMENU_H
