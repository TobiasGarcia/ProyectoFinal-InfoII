#ifndef FIREBALL_H
#define FIREBALL_H

#include <QTimer>
#include <QGraphicsPixmapItem>

#include "level/special_enemies.h"
#include "level/powerup.h"
#include "level/enemy.h"

//Esta clase modela las bolas de fuego que lanza el jugador para atacar a los enemigos.

class FireBall: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QPixmap *pix;
    QTimer *update_timer;
    QList<QGraphicsItem*> collisions;

    bool erase;

    QRectF boundingRect() const;
    QPainterPath shape() const;

public:
    FireBall(short x, short y);
    ~FireBall();

    void test_collisions();

public slots:
    void update();

};

#endif // FIREBALL_H
