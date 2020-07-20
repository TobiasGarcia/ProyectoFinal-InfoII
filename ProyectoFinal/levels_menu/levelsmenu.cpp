#include "levelsmenu.h"
#include <QDebug>

void LevelsMenu::add_fire_ball(short x, short y) {
    addItem(new FireBall(x, y));

    if (QRectF(314, 480.5, 149, 98).contains(x, y)) needle->point_direction(2);//Level 0
    else if (QRectF(589, 250.5, 149, 98).contains(x, y)) needle->point_direction(3);//Level 1
    else if (QRectF(314, 20.5, 149, 98).contains(x, y)) needle->point_direction(0);//Level 2
    else if (QRectF(39, 250.5, 149, 98).contains(x, y)) needle->point_direction(1);//Level 3
}

void LevelsMenu::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = true;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = true;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = true;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = true;

    else if (event->key() == Qt::Key_W) player2->move_dir[0] = true;
    else if (event->key() == Qt::Key_A) player2->move_dir[1] = true;
    else if (event->key() == Qt::Key_S) player2->move_dir[2] = true;
    else if (event->key() == Qt::Key_D) player2->move_dir[3] = true;

    //Qt::Key_Return es el enter cercano a las flechas, Qt::Key_Enter es el del Numeric Keypad.

    else if (!player1->delay_timer->isActive() and (event->key() == Qt::Key_Return)) add_fire_ball(player1->x(), player1->y());
    else if (!player2->delay_timer->isActive() and (event->key() == Qt::Key_Space)) add_fire_ball(player2->x(), player2->y());
}

void LevelsMenu::keyReleaseEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = false;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = false;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = false;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = false;

    else if (event->key() == Qt::Key_W) player2->move_dir[0] = false;
    else if (event->key() == Qt::Key_A) player2->move_dir[1] = false;
    else if (event->key() == Qt::Key_S) player2->move_dir[2] = false;
    else if (event->key() == Qt::Key_D) player2->move_dir[3] = false;
}

LevelsMenu::LevelsMenu(bool _two_players) : two_players(_two_players) {

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/leves_menu/resources/images/levels_menu/backgorund.png")));

    levels =  new QGraphicsPixmapItem[4];

    short gap[] = {0, 1, 0, -1};
    for (short i = 0; i < 4; i++) {
        levels[i].setPixmap(QPixmap(":/leves_menu/resources/images/levels_menu/level" + QString::number(i) + ".png"));
        levels[i].setOffset(-78, -52.5);
        levels[i].setPos(389 + gap[i]*275, 299 + gap[(i + 1)%4]*230);
        addItem(levels + i);
    }

    needle = new Needle;
    addItem(needle);

    player1 = new Player(8, 1);
    addItem(player1);

    if (two_players) {
        player2 =  new Player(8, 11, false);
        addItem(player2);
    }


//    QGraphicsRectItem *rect = new QGraphicsRectItem(314, 480.5, 149, 98);
//    addItem(rect);

//    rect = new QGraphicsRectItem(589, 250.5, 149, 98);
//    addItem(rect);

//    rect = new QGraphicsRectItem(314, 20.5, 149, 98);
//    addItem(rect);

//    rect = new QGraphicsRectItem(39, 250.5, 149, 98);
//    addItem(rect);

//    QGraphicsLineItem *line;
//    for (short i = 0; i < 9; i++) {
//        line = new QGraphicsLineItem(0, 60*(i + 1) - 1, 779, 60*(i + 1) - 1);
//        addItem(line);
//    }
//    for (short j = 0; j < 12; j++) {
//        line = new QGraphicsLineItem(60*(j + 1) - 1, 0, 60*(j + 1) - 1, 599);
//        addItem(line);
//    }
}

LevelsMenu::~LevelsMenu() {
    delete[] levels;
    delete player1;
    if (two_players) delete player2;
    delete needle;
}
