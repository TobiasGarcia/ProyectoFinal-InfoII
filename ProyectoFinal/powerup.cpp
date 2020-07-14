#include "powerup.h"
#include <QDebug>

void PowerUp::initialize() {

    if (path_type == 0) {
        a = 129; b = 69; spd = 15;

        //Ángulo aleatorio entre 0 y 2*pi.
        angle = (rand()%360)*M_PI/180;
        period = 2*M_PI;
        r = radio();

        setPos(r*cos(angle), r*sin(angle));
    }
    else if (path_type == 1) {
        a = 260; spd = 15;

        angle = 1;
        period = 2*M_PI;
        r = radio();

        setPos(r*cos(angle), r*sin(angle));
    }
    else if (path_type == 2) {
        a = 250; b = 5.0/3.0; spd = 15;

        angle = (rand()%540)*M_PI/180;
        period = 3*M_PI;
        r = radio();

        setPos(r*cos(angle), r*sin(angle));
    }
    else if (path_type == 3) {
        a = 300; b = 2*(rand()%2) - 1; spd = 15;

        //Ángulo aleatorio entre -pi/4 y pi/4.
        angle = (rand()%61 - 30)*M_PI/180;
        r = a*b*sqrt(cos(2*angle));

        setPos(r*cos(angle), r*sin(angle));
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

PowerUp::PowerUp(short _path_type, short _power_type) : path_type(_path_type), power_type(_power_type) {

   pix = new QPixmap(":/images/resources/images/normal.png");
   setPixmap(*pix);
   setOffset(-11, -11);
   setZValue(2);

   initialize();

   move_timer = new QTimer;

   if (path_type != 3) connect(move_timer, &QTimer::timeout, this, &PowerUp::move);
   else connect(move_timer, &QTimer::timeout, this, &PowerUp::lemniscate_move);

   move_timer->start(50);
}

PowerUp::~PowerUp() {
    delete pix;
    delete move_timer;
}

void PowerUp::move() {

    dr = diff_radio();
    angle += spd/sqrt(dr*dr + r*r);
    if (angle > period) angle -= period;

    r = radio();
    setPos(389 + r*cos(angle), 269 - r*sin(angle));
}

void PowerUp::lemniscate_move() {

    if (abs(r) < 30) {

        if (!straight_line) {
            b *= -1;
            angle +=  b*spd/sqrt(dr*dr + r*r);
            m_angle = atan2(269 - y(), x() - 389);
        }

        straight_line = true;
        setPos(x() - spd*cos(m_angle), y() + spd*sin(m_angle));
        r = sqrt(pow(x() - 389, 2) + pow(269 - y(), 2));
    }
    else {

        //En realidad la derivada de la lemniscata va con un signo menos, pero como
        //nos interesa elevarla al cuadrado es indiferente el colocarlo o no.

        straight_line = false;
        dr = a*sin(2*angle)/sqrt(cos(2*angle));
        angle += b*spd/sqrt(dr*dr + r*r);

        r = a*b*sqrt(cos(2*angle));
        setPos(389 + cos(angle)*r, 269 - sin(angle)*r);
    }
}
