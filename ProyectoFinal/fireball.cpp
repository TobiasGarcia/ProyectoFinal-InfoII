#include "fireball.h"
#include <QDebug>

FireBall::FireBall(short x, short y) {

    erase = false;

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/images/resources/images/fire_ball0.png");
    pix[1] = QPixmap(":/images/resources/images/fire_ball1.png");

    setPos(x - 17.5, y - 17.5);
    setPixmap(pix[0]);
    setZValue(4);

    update_timer = new QTimer;
    connect(update_timer, &QTimer::timeout, this, &FireBall::update);
    update_timer->start(150);
}

FireBall::~FireBall() {
    delete[] pix;
    delete update_timer;
}

void FireBall::test_collisions() {

    QGraphicsItem *item;
    collisions = collidingItems(Qt::IntersectsItemShape);
    for (short i = 0; i < collisions.size(); i++) {
        item = collisions[i];
        if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail)) or (typeid(*item) == typeid(Porcupine))
         or (typeid(*item) == typeid(Owl)) or (typeid(*item) == typeid(Chamaleon)) or (typeid(*item) == typeid(Mole))
         or (typeid(*item) == typeid(Vulture))) {
            Enemy *enemy = dynamic_cast<Enemy*>(item);
            enemy->reduces_health(100);
            return;
        }
        else if (typeid(*item) == typeid(PowerUp)) {
            PowerUp *power_up = dynamic_cast<PowerUp*>(item);

            //No es buena idea andar conectando las fire balls siempre, por eso utilzamos
            //el método para emitir la señal.

            power_up->emit_give_power();
            scene()->removeItem(power_up);
            delete power_up;

            return;
        }
    }
}

QRectF FireBall::boundingRect() const {
    return QRectF(0, 0, 35, 35);
}

QPainterPath FireBall::shape() const {
    QPainterPath path;
    path.addEllipse(QRect(5, 5, 25, 25));
    return path;
}

void FireBall::update() {

    if (!erase) {
        setPixmap(pix[1]);
        erase = true;
    }
    else delete this;
}
