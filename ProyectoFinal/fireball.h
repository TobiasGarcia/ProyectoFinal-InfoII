#ifndef FIREBALL_H
#define FIREBALL_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "level/enemy.h"
#include "level/powerup.h"
#include "level/special_enemies.h"

class FireBall: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    bool erase;
    QPixmap *pix;
    QTimer *update_timer;
    QList<QGraphicsItem*> collisions;
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
