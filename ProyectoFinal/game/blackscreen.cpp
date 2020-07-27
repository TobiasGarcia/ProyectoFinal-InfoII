#include "blackscreen.h"
#include <QDebug>

BlackScreen::BlackScreen() {

    setRect(0, 0, 779, 599);
    setBrush(Qt::black);
    setZValue(8);
    opacity = 100;

    opacity_timer = new QTimer;
    connect(opacity_timer, &QTimer::timeout, this, &BlackScreen::update_opacity);
}

BlackScreen::~BlackScreen() {
    delete opacity_timer;
}

void BlackScreen::change_opacity(bool _increase_opacity) {
    increase_opacity = _increase_opacity;
    opacity_timer->start(40);
}

void BlackScreen::update_opacity() {

    if (increase_opacity) opacity += 5;
    else opacity -= 5;

    if ((opacity < 0) or (100 < opacity)) {
        opacity_timer->stop();
        if (increase_opacity) emit finish();
    }
    else setOpacity(opacity/100.0);
}
