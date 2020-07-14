#include "base.h"
#include <QDebug>

Base::Base(QGraphicsRectItem *_health_bar) : health_bar(_health_bar) {

    health = 1000;

    pix = new QPixmap(":/images/resources/images/base.png");
    setPos(360, 240);
    setPixmap(*pix);
    setZValue(0);

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/images/resources/images/normal.png");
    pix[1] = QPixmap(":/images/resources/images/bitten.png");

    center = new QGraphicsPixmapItem(pix[0], this);
    center->setPos(19, 19);

    bitten_timer = new QTimer;
    bitten_timer->setSingleShot(true);
    connect(bitten_timer, &QTimer::timeout, this, &Base::return_normal);
}

Base::~Base() {
    delete center;
    delete[] pix;
    delete bitten_timer;
}

void Base::bitten() {
    health -= 10;
    health_bar->setRect(221, 544, 337*(health/1000.0), 51);
    center->setPixmap(pix[1]);
    bitten_timer->start(500);
}

void Base::return_normal() {
    center->setPixmap(pix[0]);
}








































