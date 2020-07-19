#ifndef MINIGAME_H
#define MINIGAME_H

#include <QGraphicsScene>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <math.h>
#include <ball.h>

class Minigame: public QGraphicsScene {
private:
    Ball *ball;
    double time, ang_frecuency;
    QTimer *claw_move_timer;
    bool claw_close;
    QPixmap *pix;
    QGraphicsPixmapItem *claw;

    void keyPressEvent(QKeyEvent *event);
    void display_walls();

public:
    Minigame();
    ~Minigame();
public slots:
    void claw_move();
};

#endif // MINIGAME_H
