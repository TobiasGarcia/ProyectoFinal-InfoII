#include "needle.h"
#include <QDebug>

QRectF Needle::boundingRect() const {
    return QRectF(-22, -127, 44, 254);
}

void Needle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Esto es para que se vea con buena resolución al realizar rotaciones.
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(-22, -127, *pix);
}

Needle::Needle() {

    pix = new QPixmap(":/leves_menu/resources/images/levels_menu/needle.png");
    setPos(390, 300);
    setPixmap(*pix);

    amp = 75;
    time = 0;
    gamma = 0;
    phase = 0;
    release_time = 0;

    //Para un periodo T = 4 segundos, la frecuencia angular corresponde
    //a 2*pi/4 = 1.570796, aproximadamente.
    ang_frecuency = 2*M_PI/4;

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Needle::move);
    move_timer->start(50);
}

Needle::~Needle() {
    delete pix;
    //delete move_timer;
}

void Needle::point_direction(short direction) {

    Q_UNUSED(direction);

    double initial_angle = amp*cos(ang_frecuency*time), initial_speed = -ang_frecuency*amp*sin(ang_frecuency*time);

    gamma = 2;
    ang_frecuency = 2*M_PI/0.8;
    amp = sqrt(initial_angle*initial_angle + pow(initial_speed + gamma*initial_angle, 2)/(ang_frecuency*ang_frecuency));

    if ((initial_speed + gamma*initial_angle) > 0) phase = -acos(initial_angle/amp);
    else phase = acos(initial_angle/amp);

    time = 0;

    //qDebug() << amp*cos(ang_frecuency*time);
    //qDebug() << -ang_frecuency*amp*sin(ang_frecuency*time);
    //qDebug() << amp*cos(phase);
    //qDebug() << amp*(-gamma*exp(-gamma*time)*cos(ang_frecuency*time + phase) - ang_frecuency*exp(-gamma*time)*sin(ang_frecuency*time + phase));
}

void Needle::move() {
    time += 0.06;
    if (time > 1000) return;

    setRotation(180 - amp*exp(-gamma*time)*cos(ang_frecuency*time + phase));
}
