#ifndef LEVELSMENU_H
#define LEVELSMENU_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include "player.h"
#include "fireball.h"
#include "levels_menu/needle.h"

class LevelsMenu: public QGraphicsScene {
private:
    Needle *needle;
    bool two_players;
    Player *player1, *player2;
    QGraphicsPixmapItem *levels;

    void add_fire_ball(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public:
    LevelsMenu(bool _two_players);
    ~LevelsMenu();
};

#endif // LEVELSMENU_H
