#include "minigame.h"
#include <QDebug>

void Minigame::keyPressEvent(QKeyEvent *event) {

    Q_UNUSED(event);

//    if (claw_open) {
//        claw->setPixmap(pix[0]);
//        claw->setOffset(-33, 0);
//        claw_open = false;
//    }
//    else {
//        claw->setPixmap(pix[1]);
//        claw->setOffset(-50, 0);
//        claw_open = true;
//    }

    if (claw_close) {
        claw->setPixmap(pix[1]);
        claw->setOffset(-50, 0);
        claw_close = false;
        ball->start_falling(155*ang_frecuency*cos(ang_frecuency*time));
        //claw_move_timer->stop();
    }
}

void Minigame::display_walls() {

    QGraphicsRectItem *wall = new QGraphicsRectItem(0, 0, 179, 600);
    wall->setBrush(QColor(151, 115, 55));
    wall->setPen(QColor(151, 115, 55));
    addItem(wall);

    wall = new QGraphicsRectItem(600, 0, 179, 600);
    wall->setBrush(QColor(151, 115, 55));
    wall->setPen(QColor(151, 115, 55));
    addItem(wall);

    wall = new QGraphicsRectItem(180, 0, 419, 30);
    wall->setBrush(QColor(151, 115, 55));
    wall->setPen(QColor(151, 115, 55));
    addItem(wall);

    wall = new QGraphicsRectItem(180, 569, 419, 30);
    wall->setBrush(QColor(151, 115, 55));
    wall->setPen(QColor(151, 115, 55));
    addItem(wall);
}

Minigame::Minigame() {

    setSceneRect(0, 0, 779, 599); //780x600 pixeles para que los jugadores se muevan de 15 en 15.

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/minigame/resources/images/minigame/claw0.png");//-33
    pix[1] = QPixmap(":/minigame/resources/images/minigame/claw1.png");//-50

    display_walls();

    claw = new QGraphicsPixmapItem;
    claw->setPixmap(pix[0]);
    claw->setOffset(-33, 0);
    claw->setPos(389 + 155, 30);//234, 544
    addItem(claw);

    ball = new Ball(389 + 155, 103);
    addItem(ball);
    //ball->setPos(585, 300);

    claw_close = true;

    time = 0;

    //Para un periodo de tres segundos utilziamos 2*pi/3 = 2.094395.
    ang_frecuency = 2.094395;

    claw_move_timer = new QTimer;
    connect(claw_move_timer, &QTimer::timeout, this, &Minigame::claw_move);
    claw_move_timer->start(50);

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

Minigame::~Minigame() {
    delete claw;
    delete[] pix;
    delete claw_move_timer;
    delete ball;
}

void Minigame::claw_move() {
    //Sumamos de 0.05 porque dibujamos cada 0.05 segundos,
    //más 0.02 para compensar tiempos de cálculos.
    time += 0.06;
    if (time > 3) time = 0;

    claw->setX(389 + 155*sin(ang_frecuency*time));
    if (claw_close) ball->setX(claw->x());
}













































