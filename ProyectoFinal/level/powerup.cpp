#include "powerup.h"
#include <QDebug>

void PowerUp::initialize() {

    if (path_type == 0) {
        a = 129; b = 69; spd = 300;

        //Ángulo aleatorio entre 0 y 2*pi.
        angle = (rand()%360)*M_PI/180;
        period = 2*M_PI;
        r = radio();

        setPos(389 + r*cos(angle), 269 - r*sin(angle));
    }
    else if (path_type == 1) {
        a = 260; spd = 300;

        angle = ((rand()%31 + 30) + 90*(rand()%4))*M_PI/180;
        period = 2*M_PI;
        r = radio();

        setPos(389 + r*cos(angle), 269 - r*sin(angle));
    }
    else if (path_type == 2) {
        a = 250; b = 5.0/3.0; spd = 300;

        angle = (rand()%540)*M_PI/180;
        period = 3*M_PI;
        r = radio();

        setPos(389 + r*cos(angle), 269 - r*sin(angle));
    }
    else if (path_type == 3) {
        a = 300; b = 2*(rand()%2) - 1; spd = 300;

        //Ángulo aleatorio entre -pi/4 y pi/4.
        angle = (rand()%61 - 30)*M_PI/180;
        r = a*b*sqrt(cos(2*angle));

        setPos(389 + r*cos(angle), 269 - r*sin(angle));
    }
}

double PowerUp::radio() {

    if (path_type == 0) return 1/sqrt(pow(cos(angle)/a, 2) + pow(sin(angle)/b, 2));
    else if (path_type == 1) return a/pow(pow(cbrt(cos(angle)), 2) + pow(cbrt(sin(angle)), 2), 1.5);
    else return a*cos(b*angle);
}

double PowerUp::diff_radio() {

    if (path_type == 0) return (0.5*a*b*(b*b - a*a))*(sin(2*angle)/pow(pow(b*cos(angle), 2) + pow(a*sin(angle), 2) , 1.5));
    else if (path_type == 1) {
        c = cos(angle); s = sin(angle);
        return a*((s/cbrt(c)) - (c/cbrt(s)))/pow(pow(cbrt(c), 2) + pow(cbrt(s), 2), 2.5);
    }

    //En realidad la derivada de la rosa va con un signo menos, pero como
    //nos interesa elevarla al cuadrado es indiferente el colocarlo o no.

    else return a*b*sin(b*angle);
}

QPainterPath PowerUp::shape() const{
    QPainterPath path;
    path.addEllipse(QRect(-11, -11, 22, 22));
    return path;
}

//Este método es para no tener que conectar siempre todas las fireballs al slot
//del nivel, pues las fireballs como tal casi nunca lanzarian la señal pues muchas
//se usan es para los enemigos o para tirarlas a la nada, mientras que el power_up
//si es mucho más probable que lance la señal, pues está diseñado para que lo haga.

void PowerUp::emit_give_power() {
    emit give_power(power_type);
}

PowerUp::PowerUp(short _path_type, short _power_type) : path_type(_path_type), power_type(_power_type) {

    pix = new QPixmap(":/power_ups/resources/images/power_ups/power" + QString::number(power_type) + ".png");

    setPixmap(*pix);
    setOffset(-11, -11);
    setZValue(3);

    initialize();

    move_timer = new QTimer;

    if (path_type != 3) connect(move_timer, &QTimer::timeout, this, &PowerUp::move);
    else connect(move_timer, &QTimer::timeout, this, &PowerUp::lemniscate_move);

    freez = false;
    move_timer->start(50);
}

PowerUp::~PowerUp() {
    delete pix;
    delete move_timer;
}

void PowerUp::move() {

    if (freez) return;

    dr = diff_radio();
    angle += 0.06*spd/sqrt(dr*dr + r*r);
    if (angle > period) angle -= period;

    r = radio();
    setPos(389 + r*cos(angle), 269 - r*sin(angle));
}

void PowerUp::lemniscate_move() {

    //Como dibujamos cada 0.05 segundos, al discretizar la derivada del ángulo
    //debemos multiplicar por esos 0.05 segundos que es el intervalo de tiempo,
    //sin embargo, utilizamos 0.06 en su lugar para compensar el tiempo que
    //tarda la máquina en realizar los cálculos y modificar la posición
    //de la imágen en pantalla.

    //Este aumento de 0.01 fue determinado por tanteo, pues es difícil carlcular
    //cuanto tarda en promedio la máquina en realizar todo este proceso,
    //además, este número puede variar dependiendo de la máquina donde
    //se corra el código.

    if (abs(r) < 40) {

        if (!straight_line) {
            b *= -1;
            angle +=  b*0.06*spd/sqrt(dr*dr + r*r);
            m_angle = atan2(269 - y(), x() - 389);
        }

        straight_line = true;
        setPos(x() - 0.06*spd*cos(m_angle), y() + 0.06*spd*sin(m_angle));
        r = sqrt(pow(x() - 389, 2) + pow(269 - y(), 2));
    }
    else {

        //En realidad la derivada de la lemniscata va con un signo menos, pero como
        //nos interesa en particular su segunda potencia, es indiferente el colocarlo o no.

        straight_line = false;
        dr = a*sin(2*angle)/sqrt(cos(2*angle));
        angle += b*0.06*spd/sqrt(dr*dr + r*r);

        r = a*b*sqrt(cos(2*angle));
        setPos(389 + cos(angle)*r, 269 - sin(angle)*r);
    }
}
