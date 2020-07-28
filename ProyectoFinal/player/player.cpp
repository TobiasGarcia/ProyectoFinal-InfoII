#include "player.h"

QRectF Player::boundingRect() const {
    return QRectF(-18, -18, 36, 36);
}

Player::Player(short i, short j, bool level, bool _player_one) : player_one(_player_one) {

    if (player_one) pix = new QPixmap(":/players/resources/images/players/blue_scope.png");
    else pix = new QPixmap(":/players/resources/images/players/purple_scope.png");

    setPos(30 + 60*j, 30 + 60*i);
    setOffset(-15, -15);
    setPixmap(*pix);
    setZValue(6);

    //Esto es pues en el levels menu el límite inferior es "mayor".

    if (level) lower_limit = 525;
    else lower_limit = 585;

    move_dir.fill(false);
    freez = false;

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Player::move);
    move_timer->start(17);
}

Player::~Player() {
    delete pix;
    delete move_timer;
}

void Player::move() {

    if (freez) return;

    if (move_dir[0] and (y() > 15)) setY(y() - 5);
    if (move_dir[1] and (x() > 15)) setX(x() - 5);
    if (move_dir[2] and (y() < lower_limit)) setY(y() + 5);
    if (move_dir[3] and (x() < 765)) setX(x() + 5);
}
