#include "enemy.h"
#include <QDebug>

Enemy::Enemy() {

    //50 x 40 pixeles;
    width_half = 20;
    height_half = 25;

    pix = new QPixmap(":/images/resources/images/enemy.png");
    setPixmap(*pix);
    setPos(180, 560);

    spd = 50;

    targets.push_back(QVector2D(480, 540));
    targets.push_back(QVector2D(240, 480));

    dir = targets.front() - QVector2D(pos());
    speed = spd*dir.normalized();
    setRotation(90 - (atan2(-dir[1], dir[0])*180/M_PI));

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Enemy::move);
    move_timer->start(50);
}

Enemy::~Enemy() {
    delete pix;
    delete move_timer;
}

void Enemy::move() {

    setPos(x() + speed[0]*0.1, y() + speed[1]*0.1);
    dir = targets.front() - QVector2D(pos());

    if (dir.length() < 5) {
        targets.pop_front();
        if (targets.empty()) move_timer->stop();
        else {
            dir = targets.front() - QVector2D(pos());
            speed = spd*dir.normalized();
            setRotation(90 - (atan2(-dir[1], dir[0])*180/M_PI));
        }
    }
}

QRectF Enemy::boundingRect() const {
    return QRectF(-width_half, -height_half, width_half*2, height_half*2);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Esto es para que se vea con buena resolución al realizar rotaciones.
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(-width_half, -height_half, *pix);
}

































































