#include "minigame.h"
#include <QDebug>

void Minigame::keyPressEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Return) {
        if (state == 1) drop_ball();
        else if (state == 3) second_chance();
        else if (!two_players and (state == 5)) drop_ball();
        else if (state == 7) qDebug() << "Finish";
    }
    else if (two_players and (event->key() == Qt::Key_V) and (state == 5)) drop_ball();
    else if (two_players and (event->key() == Qt::Key_V) and (state == 7)) qDebug() << "Finish";
}

//void Minigame::display_walls() {

//    QGraphicsRectItem *wall = new QGraphicsRectItem(0, 0, 179, 600);
//    wall->setBrush(QColor(151, 115, 55));
//    wall->setPen(QColor(151, 115, 55));
//    addItem(wall);

//    wall = new QGraphicsRectItem(600, 0, 179, 600);
//    wall->setBrush(QColor(151, 115, 55));
//    wall->setPen(QColor(151, 115, 55));
//    addItem(wall);

//    wall = new QGraphicsRectItem(180, 0, 419, 30);
//    wall->setBrush(QColor(151, 115, 55));
//    wall->setPen(QColor(151, 115, 55));
//    addItem(wall);

//    wall = new QGraphicsRectItem(180, 569, 419, 30);
//    wall->setBrush(QColor(151, 115, 55));
//    wall->setPen(QColor(151, 115, 55));
//    addItem(wall);
//}

void Minigame::fail() {
    claw_move_timer->stop();
    ball->stop_timer();

    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw_close = true;

    information->display_message(389, 250, QString("¡Tiempo Fuera!"));
    state++;
}

void Minigame::second_chance() {

    removeItem(information);

    removeItem(ball);
    delete ball;

    goal->setPos(rand()%381 + 200, 440);
    claw->setPos(389, 30);

    ball = new Ball(389, 103, goal->x(), goal->y());
    connect(ball, &Ball::win, this, &Minigame::win);
    addItem(ball);

    time = 0;
    gamma = 0;
    release_time = 0;

    if (two_players) {
        message = "     Segunda Oportunidad\n"
                  "Segundo Jugador (Tecla V)";
    }
    else message = "Segunda Oportunidad";

    information->display_message(389, 250, message);
    delay_timer->start(3000);
    state++;
}

void Minigame::drop_ball() {
    claw->setPixmap(claw_pix[1]);
    claw->setOffset(-50, 0);
    claw_close = false;

    ball->start_falling(155*ang_frecuency*cos(ang_frecuency*time));

    //Queremos que la frecuencia angular durante el amortiguamiento sea la misma que antes
    //de este, por lo cual basta con utilizar la misma frecuencia angular, y de todas formas
    //podemos elegir la masa y el coeficiente b, la proporción entre la fricción y la rapidez,
    //como queramos, pues para cualquier combinación de valores positivos de estos siempre
    //existirá un k (la constante del "resorte") que permitrá este movimiento.

    //En este caso tomamos 10 kg de masa y una b de 10 kg/s, para una gamma de 10/(2*10) = 0.5.

    gamma = 0.5;
    release_time = time;

    state++;
}

Minigame::Minigame(short _two_players) : two_players(_two_players) {

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/minigame/resources/images/minigame/minigame_background.png")));

    //La variable state nos ayudará a manejar el flujo del minijuego indicándonos en que estado nos
    //encontramos; los estados posibles son los siguientes:

    //0: Estamos mostrando un mensaje y esperamos unos segundos antes de empezar.
    //1: Estamos en la primera oportunidad para jugar el minijuego, espramos a que se suelte la pelota.
    //2: Ya se soltó la pelota y está rebotando, esperamos a que se gane o se pierda.
    //3: Se muestra la información respecto a haber ganado o perdido; esperamos a que se presione la tecla enter.
    //4: Mostramos un mensaje y esperamos unos segundos antes de la segunda oportunidad.
    //5: Estamos en la segunda oportunidad para jugar el minijuego, espramos a que se suelte la pelota.
    //6: Ya se soltó la pelota y está rebotando, esperamos a que se gane o se pierda.
    //7: Se muestra la información respecto a haber ganado o perdido; esperamos a que se presione la tecla enter.

    state = 0;

    information = new Information(this);

    if (two_players) {
        message = "                  MINIJUEGO\n"
                  "             ¡Dale al Objetivo!\n"
                  "Primer Jugador (Tecla Enter)";
    }
    else {
        message = "      MINIJUEGO\n"
                  "¡Dale al Objetivo!";
    }

    information->display_message(389, 240, message);

    claw_pix = new QPixmap[2];
    claw_pix[0] = QPixmap(":/minigame/resources/images/minigame/claw0.png");//-33
    claw_pix[1] = QPixmap(":/minigame/resources/images/minigame/claw1.png");//-50

    claw = new QGraphicsPixmapItem;
    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw->setPos(389, 30);
    claw_close = true;
    addItem(claw);

    goal = new QGraphicsPixmapItem(QPixmap(":/minigame/resources/images/minigame/goal.png"));
    goal->setOffset(-10, -10);
    goal->setPos(rand()%381 + 200, 440);//200, 580
    addItem(goal);

    ball = new Ball(389, 103, goal->x(), goal->y());
    connect(ball, &Ball::win, this, &Minigame::win);
    addItem(ball);

    //Para un periodo T = 3 segundos, la frecuencia angular corresponde
    //a 2*pi/3 = 2.094395, aproximadamente.
    ang_frecuency = 2*M_PI/3;

    //Comenzamos con un gamma de 0 para no considerar la fricción hasta que se
    //suelte la pelota.
    gamma = 0;

    time = 0;
    release_time = 0;

    claw_move_timer = new QTimer;
    connect(claw_move_timer, &QTimer::timeout, this, &Minigame::claw_move);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Minigame::next_state);
    delay_timer->start(4000);

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
    delete[] claw_pix;
    delete claw_move_timer;
    delete ball;
    delete goal;
    delete delay_timer;
}

void Minigame::claw_move() {
    //Sumamos de 0.05 porque dibujamos cada 0.05 segundos,
    //más 0.02 para compensar tiempos de cálculos.
    time += 0.06;
    if ((claw_close) and (time > 3)) time = 0;

    claw->setX(389 + 155*exp(-gamma*(time - release_time))*sin(ang_frecuency*time));
    if (claw_close) ball->setX(claw->x());

    if ((!claw_close) and (exp(-gamma*(time - release_time)) < 0.02)) fail();
}

void Minigame::win() {

    claw_move_timer->stop();
    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw_close = true;

    message = "     ¡MISIÓN CUMPLIDA!\n"
                      "\n"
                      "Recompensas obtenidas:\n";

    if (ball->get_rebounds() < 2) message += "   Vida extra x 1\n";
    message += "   Pegamento x 2\n"
               "   Roca x 2\n"
               "\n"
               "Número de Rebotes: " + QString::number(ball->get_rebounds());

    information->display_message(389, 150, message);
    state++;
}

void Minigame::next_state() {
    removeItem(information);
    claw_move_timer->start(50);
    state++;
}














































