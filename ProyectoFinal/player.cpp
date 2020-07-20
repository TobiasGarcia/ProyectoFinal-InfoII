#include "player.h"

QRectF Player::boundingRect() const {
    return QRectF(-18, -18, 36, 36);
}

Player::Player(short i, short j, bool _player_one) : player_one(_player_one) {

    if (player_one) pix = new QPixmap(":/images/resources/images/blue_scope.png");
    else pix = new QPixmap(":/images/resources/images/purple_scope.png");

    setPos(30 + 60*j, 30 + 60*i);
    setOffset(-15, -15);
    setPixmap(*pix);
    setZValue(6);

    move_dir.fill(false);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Player::move);
    move_timer->start(17);
}

Player::~Player() {
    delete pix;
    delete delay_timer;
}

void Player::move() {
    if (move_dir[0] and (y() > 15)) setY(y() - 5);
    if (move_dir[1] and (x() > 15)) setX(x() - 5);
    if (move_dir[2] and (y() < 525)) setY(y() + 5);
    if (move_dir[3] and (x() < 765)) setX(x() + 5);
}