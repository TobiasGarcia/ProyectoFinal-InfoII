#include "base.h"
#include <QDebug>

Base::Base(QGraphicsRectItem *_health_bar, short *_health, bool *_extra_life,
           QGraphicsPixmapItem *_lifebuoy, bool _tutorial_level) :

           lifebuoy(_lifebuoy), extra_life(_extra_life), tutorial_level(_tutorial_level),
           health_bar(_health_bar), health(_health) {

    vulnerable = true;

    pix = new QPixmap(":/base/resources/images/base/base.png");
    setPos(360, 240);
    setPixmap(*pix);
    setZValue(0);

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/base/resources/images/base/normal.png");
    pix[1] = QPixmap(":/base/resources/images/base/bitten.png");

    center = new QGraphicsPixmapItem(pix[0], this);
    center->setPos(19, 19);

    hurt_timer = new QTimer;
    hurt_timer->setSingleShot(true);
    connect(hurt_timer, &QTimer::timeout, this, &Base::return_normal);
}

Base::~Base() {
    delete center;
    delete[] pix;
    delete hurt_timer;
}

void Base::increase_health(short increment) {
    (*health) += increment;
    if ((*health) > 1000) (*health) = 1000;
    health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
}

void Base::bitten() {
    if (vulnerable) {

        (*health) -= 10;

        if (((*health) < 300) and (*extra_life)) {

            //No permitimos que en el nivel tutorial se pierda.

            if (!tutorial_level) {
                scene()->removeItem(lifebuoy);
                (*extra_life) = false;
            }

            hurt_timer->start(500);
            (*health) = 1000;
        }
        else if ((*health) == 0) {
            if (hurt_timer->isActive()) hurt_timer->stop();
            vulnerable = false;
            emit no_health();
        }
        else hurt_timer->start(500);

        health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
        center->setPixmap(pix[1]);
    }
}

void Base::return_normal() {
    center->setPixmap(pix[0]);
}

void Base::vulture_hit() {

    if ((*extra_life)) {
        scene()->removeItem(lifebuoy);
        hurt_timer->start(500);
        (*extra_life) = false;
        (*health) = 1000;

        health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
        center->setPixmap(pix[1]);
    }
    else {
        (*health) = 0;

        if (hurt_timer->isActive()) hurt_timer->stop();
        vulnerable = false;
        emit no_health();

        health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
        center->setPixmap(pix[1]);
    }
}









































