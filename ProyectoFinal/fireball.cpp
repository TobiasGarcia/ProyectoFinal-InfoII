#include "fireball.h"
#include <QDebug>

FireBall::FireBall(short x, short y) {

    erase = false;

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/images/resources/images/fire_ball0.png");
    pix[1] = QPixmap(":/images/resources/images/fire_ball1.png");

    setPos(x - 17.5, y - 17.5);
    setPixmap(pix[0]);
    setZValue(2);

    update_timer = new QTimer;
    connect(update_timer, &QTimer::timeout, this, &FireBall::update);
    update_timer->start(150);
}

FireBall::~FireBall() {
    delete[] pix;
    delete update_timer;
}

void FireBall::test_collisions() {

    collisions = collidingItems(Qt::IntersectsItemShape);
    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
//        if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail)) or (typeid(*item) == typeid(Enemy))
//         or (typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Enemy))
//         or (typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Enemy))) {
//            Enemy *enemy = dynamic_cast<Enemy*>(item);
//            enemy->reduces_health();
//            return;
//        }
        if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail)) or (typeid(*item) == typeid(Porcupine))
         or (typeid(*item) == typeid(Owl)) or (typeid(*item) == typeid(Chamaleon)) or (typeid(*item) == typeid(Mole))
         or (typeid(*item) == typeid(Vulture))) {
            Enemy *enemy = dynamic_cast<Enemy*>(item);
            enemy->reduces_health();
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
