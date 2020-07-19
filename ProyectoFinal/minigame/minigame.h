#ifndef MINIGAME_H
#define MINIGAME_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <math.h>
#include <minigame/ball.h>
#include <information.h>

class Minigame: public QGraphicsScene {
private:
    QString message;
    Ball *ball;
    short state;
    double time, ang_frecuency, gamma, release_time;
    QTimer *claw_move_timer, *delay_timer;
    bool two_players, claw_close;
    QPixmap *claw_pix;
    Information *information;
    QGraphicsPixmapItem *claw, *goal;

    void keyPressEvent(QKeyEvent *event);
    void fail();
    void second_chance();
    void drop_ball();

public:
    Minigame(short _two_players);
    ~Minigame();
public slots:
    void claw_move();
    void win();
    void next_state();
};

#endif // MINIGAME_H
