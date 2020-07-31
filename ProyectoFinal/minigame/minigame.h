#ifndef MINIGAME_H
#define MINIGAME_H

#include <math.h>

#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <game/information.h>
#include <game/blackscreen.h>

#include <minigame/ball.h>

//Esta clase modela el minijuego del que se le permite participar al jugador al final
//de cada nivel, o el tutorial.

class Minigame: public QGraphicsScene {

    Q_OBJECT

    //En el minijuego habrá una garra que estará describiendo un movimiento armónico simple (MAS), y que cuando
    //el jugador presiona una tecla determinada, soltará una pelota que describirá un movimiento parabólico y
    //que rebotará contra las paredes aplicando un coeficiente de restitución, luego de que la garra libere
    //la pelota, esta pasará de describir un MAS a una oscilación subamortiguada.

    //La física de la pelota está programada dentro de la clase Ball, en esta clase se programa
    //toda la física de la garra y el flujo del minijuego.

private:
    Ball *ball;
    Information *information;
    BlackScreen *black_screen;

    QTimer *claw_move_timer, *delay_timer;
    QGraphicsPixmapItem *claw, *goal;
    QPixmap *claw_pix;
    QString message;

    short state, *rocks_num, *glues_num;
    bool two_players, claw_close, *extra_life;

    //Utilizaremos la variable time para llevar la cuenta del tiempo transcurrido; ang_frecuency almacenará
    //la frecuencia angular del MAS; el valor de la variable gamma será un parámetro del movimiento
    //subamortiguado; la variable phase resulta de utilidad para realizar la transición del MAS al
    //movimiento subamortiguado; la amplitud de la oscilación será almacenada en la variable amp;
    //a diferencia del movimiento de la aguja de la brújula dentro del menú de niveles, la garra
    //sí simulará el movimiento de un sistema masa-resorte, por lo cual utilizaremos las
    //variables k, b y m para almacenar los parámetros del movimiento; la variable
    //period almacenará el período de la oscilación.

    double time, ang_frecuency, gamma, phase, amp, k, b, m, period;

    void keyPressEvent(QKeyEvent *event);

    void second_chance();

    void drop_ball();

    void fail();

    void initialize_objects();

    void display_initial_message();

public:
    Minigame(short _two_players, short *_rocks_num, short *_glues_num, bool *_extra_life);
    ~Minigame();

public slots:
    void win();
    void claw_move();
    void next_state();

signals:
    void minigame_finished();

};

#endif // MINIGAME_H
