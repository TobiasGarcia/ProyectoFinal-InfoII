#ifndef LEVELSMENU_H
#define LEVELSMENU_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "player/player.h"
#include "player/fireball.h"
#include "levels_menu/needle.h"
#include "game/blackscreen.h"

class LevelsMenu: public QGraphicsScene {

    Q_OBJECT

private:
    short state, level_num, *levels_waves;
    Needle *needle;
    bool two_players;
    Player *player1, *player2;
    BlackScreen *black_screen;

    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

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
