#ifndef BALL_H
#define BALL_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector2D>
#include <math.h>

class Ball: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //Tomaremos la gravedad en dirección del eje Y positivo, pues esta es la dirección
    //que los usuarios perciben como abajo.

    double restitution, time, specific_energy, scaled_gravity;
    QVector2D initial_speed, initial_pos;
    QTimer *move_timer;
    QList<QGraphicsItem*> collisions;

    QRectF boundingRect() const;
    QPainterPath shape() const;

public:
    Ball(short x, short y);
    ~Ball();

    void start_falling(double spd);
    void collision(short side);
public slots:
    void move();
};

#endif // BALL_H
