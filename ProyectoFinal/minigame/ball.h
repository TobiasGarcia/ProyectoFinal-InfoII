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

    double initial_speed[2], initial_pos[2], restitution, time, specific_energy, scaled_gravity;
    short goal_pos[2], rebounds_num;
    QTimer *move_timer;
    QList<QGraphicsItem*> collisions;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void collision(short side);

public:
    Ball(short x, short y, short goal_x, short goal_y);
    ~Ball();

    void start_falling(double spd);
    void stop_timer();
public slots:
    void move();
    short get_rebounds() {return rebounds_num;};
signals:
    void win();
};

#endif // BALL_H
