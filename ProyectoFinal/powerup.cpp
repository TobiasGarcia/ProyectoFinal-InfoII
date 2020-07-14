#include "powerup.h"
#include <QDebug>

double PowerUp::radio() {

    if (path_type == 0) return 1/sqrt(pow(cos(angle)/a, 2) + pow(sin(angle)/b, 2));
    else return 2;
}

double PowerUp::diff_radio() {

    if (path_type == 0) return (0.5*a*b*(b*b - a*a))*(sin(2*angle)/pow(pow(b*cos(angle), 2) + pow(a*sin(angle), 2) , 1.5));
    else return 2;
}

PowerUp::PowerUp(short _path_type, short _power_type) : path_type(_path_type), power_type(_power_type) {

   pix = new QPixmap(":/images/resources/images/normal.png");
   setPixmap(*pix);
   setOffset(-11, -11);
   setZValue(2);

   if (path_type == 0) {
       a = 330;
       b = 210;

       spd = 10;

       //Ángulo aleatorio entre 0 y 2*pi.
       angle = (rand()%360)*M_PI/180;
       r = radio();
       setPos(r*cos(angle), r*sin(angle));
   }
   else if (path_type == 3) {
       a = 400;
       b = 1;
       r = a;

       spd = 10;
       angle = 0;
   }

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

    r = radio();
    setPos(389 + r*cos(angle), 269 - r*sin(angle));
}

void PowerUp::lemniscate_move() {

    if (abs(r) < 20) {

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

        //En realidad la derivada va con un signo menos, pero como
        //nos interesa elevarla al cuadrado es indiferente
        //el colocarlo o no.

        straight_line = false;
        dr = a*sin(2*angle)/sqrt(cos(2*angle));
        angle += b*spd/sqrt(dr*dr + r*r);

        r = a*b*sqrt(cos(2*angle));
        setPos(389 + cos(angle)*r, 269 - sin(angle)*r);
    }
}
