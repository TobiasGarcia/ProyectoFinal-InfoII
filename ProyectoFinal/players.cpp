#include "players.h"

void Players::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) move_dir1[0] = true;
    else if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) move_dir1[1] = true;
    else if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) move_dir1[2] = true;
    else if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) move_dir1[3] = true;

    else if ((event->key() == Qt::Key_W) and !event->isAutoRepeat()) move_dir2[0] = true;
    else if ((event->key() == Qt::Key_A) and !event->isAutoRepeat()) move_dir2[1] = true;
    else if ((event->key() == Qt::Key_S) and !event->isAutoRepeat()) move_dir2[2] = true;
    else if ((event->key() == Qt::Key_D) and !event->isAutoRepeat()) move_dir2[3] = true;
}

void Players::keyReleaseEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) move_dir1[0] = false;
    else if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) move_dir1[1] = false;
    else if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) move_dir1[2] = false;
    else if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) move_dir1[3] = false;

    else if ((event->key() == Qt::Key_W) and !event->isAutoRepeat()) move_dir2[0] = false;
    else if ((event->key() == Qt::Key_A) and !event->isAutoRepeat()) move_dir2[1] = false;
    else if ((event->key() == Qt::Key_S) and !event->isAutoRepeat()) move_dir2[2] = false;
    else if ((event->key() == Qt::Key_D) and !event->isAutoRepeat()) move_dir2[3] = false;
}

Players::Players(short x1, short y1, short x2, short y2) {

    pix1 = new QPixmap(":/images/resources/images/neptune.png");
    setOffset(-15, -15);
    setPixmap(*pix1);
    setPos(x1, y1);

    multi = new QGraphicsPixmapItem;
    pix2 = new QPixmap(":/images/resources/images/mars.png");
    multi->setOffset(-15, -15);
    multi->setPixmap(*pix2);
    multi->setPos(x2, y2);

    move_dir1.fill(false);
    move_dir2.fill(false);

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Players::move);
    move_timer->start(1000/30); //30 fps.
}

Players::~Players() {
    delete pix1;
    delete pix2;
    delete multi;
    delete move_timer;
}

void Players::move() {

    if (move_dir1[0] and (y() > 15)) setY(y() - 15);
    if (move_dir1[1] and (x() > 15)) setX(x() - 15);
    if (move_dir1[2] and (y() < 585)) setY(y() + 15);
    if (move_dir1[3] and (x() < 780)) setX(x() + 15);

    if (move_dir2[0] and (multi->y() > 15)) multi->setY(multi->y() - 15);
    if (move_dir2[1] and (multi->x() > 15)) multi->setX(multi->x() - 15);
    if (move_dir2[2] and (multi->y() < 585)) multi->setY(multi->y() + 15);
    if (move_dir2[3] and (multi->x() < 780)) multi->setX(multi->x() + 15);
}
