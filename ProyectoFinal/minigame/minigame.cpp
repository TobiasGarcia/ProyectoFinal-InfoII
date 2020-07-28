#include "minigame.h"
#include <QDebug>

void Minigame::keyPressEvent(QKeyEvent *event) {

    if (event->isAutoRepeat()) return;

    if ((event->key() == Qt::Key_Return) and (state == 1)) drop_ball();
    else if ((event->key() == Qt::Key_Return) and (state == 3)) second_chance();
    else if ((event->key() == Qt::Key_Return) and !two_players and (state == 5)) drop_ball();
    else if ((event->key() == Qt::Key_Return) and (state == 7)) black_screen->change_opacity(true);
    else if (two_players and (event->key() == Qt::Key_Space) and (state == 3)) second_chance();
    else if (two_players and (event->key() == Qt::Key_Space) and (state == 5)) drop_ball();
    else if (two_players and (event->key() == Qt::Key_Space) and (state == 7)) black_screen->change_opacity(true);
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

    if (two_players) message = "                    ¡Tiempo Fuera!\nEnter o Espacio para continuar ->";
    else message = "         ¡Tiempo Fuera!\nEnter para continuar ->";

    information->display_message(389, 250, message);
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
    phase = 0;

    amp = 155;
    ang_frecuency = sqrt(k/m);

    if (two_players) {
        message = "            Segunda Oportunidad\n"
                  "Segundo Jugador (Tecla Espacio)";
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

    //No hace falta la fase porque en este momento es cero.
    double initial_angle = amp*sin(ang_frecuency*time),
           initial_speed = amp*ang_frecuency*cos(ang_frecuency*time);

    ball->start_falling(initial_speed);

    //Le damos un valor de 0.5 s^-1 a la variable gamma para comenzar a considerar la fricción.

    gamma = 0.5;

    //Calculamos la nueva frecuencia angular del MAS amortiguado.

    ang_frecuency = sqrt(ang_frecuency*ang_frecuency - (b*b/(4*m*m)));
    amp = sqrt(initial_angle*initial_angle + pow(initial_speed + gamma*initial_angle, 2)/(ang_frecuency*ang_frecuency));

    if ((initial_speed + gamma*initial_angle) > 0) phase = asin(initial_angle/amp);
    else phase = M_PI - asin(initial_angle/amp);

    time = 0;
    state++;
}

Minigame::Minigame(short _two_players, short *_rocks_num, short *_glues_num, bool *_extra_life) :
    rocks_num(_rocks_num), glues_num(_glues_num), two_players(_two_players), extra_life(_extra_life) {

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/minigame/resources/images/minigame/minigame_background.png")));

    black_screen = new BlackScreen;
    connect(black_screen, &BlackScreen::finish, this, &Minigame::minigame_finished);
    addItem(black_screen);

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
        message = "                    MINIJUEGO\n"
                  "             ¡Dale al Objetivo!\n"
                  "Primer Jugador (Tecla Enter)";
    }
    else {
        message = "        MINIJUEGO\n"
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

    //La garra simulará el movimiento de un sistema masa-resorte, se considerará una masa de 10 kg;
    //para que se vea bien el movimiento se usará un periodo T = 3 segundos, por lo cual la constante
    //del resorte será k = m*4*(pi^2)/(T^2) = 43.864908 N/m aproximadamente. Se utilizará una
    //amplitud inicial de 155 pixeles.

    T = 3;
    m = 10;
    k = m*4*M_PI*M_PI/(T*T);

    amp = 155;
    ang_frecuency = sqrt(k/m);

    //Comenzamos con un gamma de 0 para no considerar la fricción hasta que se
    //suelte la pelota, además de una fase inicial de 0 randianes.

    time = 0;
    gamma = 0;
    phase = 0;

    claw_move_timer = new QTimer;
    connect(claw_move_timer, &QTimer::timeout, this, &Minigame::claw_move);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Minigame::next_state);

    black_screen->change_opacity(false);
    delay_timer->start(3000);

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
    delete information;
    delete ball;
    delete goal;
    delete delay_timer;
    delete black_screen;
}

void Minigame::claw_move() {

    //Sumamos de 0.05 porque dibujamos cada 0.05 segundos,
    //más 0.02 para compensar tiempos de cálculos.

    time += 0.06;
    if ((claw_close) and (time > T)) time -= T;

    claw->setX(389 + amp*exp(-gamma*time)*sin(ang_frecuency*time + phase));
    if (claw_close) ball->setX(claw->x());

    if ((!claw_close) and (exp(-gamma*time) < 0.02)) fail();
}

void Minigame::win() {

    claw_move_timer->stop();
    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw_close = true;

    message = "      ¡MISIÓN CUMPLIDA!\n"
                      "\n"
                      "Recompensas obtenidas:\n";

    if ((ball->get_rebounds() < 2) and !(*extra_life)) {
        message += "   Vida extra x 1\n";
        *extra_life = true;
    }
    else if (ball->get_rebounds() < 2) message += "   Ya tienes una vida extra\n";

    short won_rocks = fmin(4 - (*rocks_num), 2), won_glues = fmin(4 - (*glues_num), 2);

    if (won_glues == 0) message += "   Ya tienes 4 pegamentos\n";
    else {
        message += "   Pegamento x ";
        message.push_back(char(won_glues + 48));
        message += "\n";
    }

    if (won_rocks == 0) message += "   Ya tienes 4 rocas\n";
    else {
        message += "   Rocas x ";
        message.push_back(char(won_rocks + 48));
        message += "\n";
    }

    message += "\nNúmero de Rebotes: " + QString::number(ball->get_rebounds());

    if (two_players) message += "\nEnter o Espacio para continuar ->";
    else message += "\nEnter para continuar ->";

    (*glues_num) += won_glues;
    (*rocks_num) += won_rocks;

    information->display_message(389, 150, message);
    state++;
}

void Minigame::next_state() {
    removeItem(information);
    claw_move_timer->start(50);
    state++;
}














































