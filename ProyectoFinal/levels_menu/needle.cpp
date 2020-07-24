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

    //La aguja oscilará describiendo un MAS y luego un MAS amortiguado, se utilizará un
    //período T = 4 segundos por lo cual la frecuencia angular será 2*pi/T = 1.570796 aproximadamente.
    //Se considerará además una amplitud de 75 grados.

    T = 4;
    amp = 75;
    ang_frecuency = 2*M_PI/T;

    //Comenzamos con un gamma de 0 para no considerar la fricción hasta que se
    //elija un nivel, además de una fase inicial de 0 randianes.

    time = 0;
    gamma = 0;
    phase = 0;

    offset = 0;

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Needle::move);
    move_timer->start(50);
}

Needle::~Needle() {
    delete pix;
    delete move_timer;
}

void Needle::point_direction(short direction) {

    //No hace falta la fase porque en este momento es cero.
    double initial_angle = amp*cos(ang_frecuency*time),
            initial_speed = -ang_frecuency*amp*sin(ang_frecuency*time);

    //La variable offset es para trasladar la oscilación desde el extremo inferior
    //hasta uno de los costados o la parte superior.

    if (direction == 3) offset = 90;
    else if (direction == 1) offset = -90;
    else if ((direction == 0) and (initial_angle >= 0)) offset = 180;
    else if ((direction == 0) and (initial_angle < 0)) offset = -180;

    initial_angle -= offset;

    //Le damos un valor de 2 s^-1 a la variable gamma para comenzar a considerar la fricción.

    gamma = 2;

    //Para el MAS amortiguado se utilizará un período T = 0.8 segundos,
    //de esta forma la nueva frecuencia es 2*pi/0.8 = 7.853982 aproximadamente.

    T = 0.8;
    ang_frecuency = 2*M_PI/T;
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
    if ((gamma == 0) and (time > T)) time = 0;

    setRotation(180 - (amp*exp(-gamma*time)*cos(ang_frecuency*time + phase) + offset));

    if ((gamma != 0) and (amp*exp(-gamma*time) < 0.8)) {
        move_timer->stop();
        emit finish();
    }
}
