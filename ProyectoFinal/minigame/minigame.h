#ifndef MINIGAME_H
#define MINIGAME_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <math.h>
#include <minigame/ball.h>
#include <information.h>
#include <blackscreen.h>

class Minigame: public QGraphicsScene {

    Q_OBJECT

private:
    BlackScreen *black_screen;
    QString message;
    Ball *ball;
    short state, *rocks_num, *fluids_num;
    double time, ang_frecuency, gamma, phase, amp, k, b, m, T;
    QTimer *claw_move_timer, *delay_timer;
    bool two_players, claw_close, *extra_life;
    QPixmap *claw_pix;
    Information *information;
    QGraphicsPixmapItem *claw, *goal;

    void keyPressEvent(QKeyEvent *event);
    void fail();
    void second_chance();
    void drop_ball();

public:
    Minigame(short _two_players, short *_rocks_num, short *_fluids_num, bool *_extra_life);
    ~Minigame();
public slots:
    void claw_move();
    void win();
    void next_state();
signals:
    void minigame_finished();
};

#endif // MINIGAME_H
