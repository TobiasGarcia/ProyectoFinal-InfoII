#ifndef BALL_H
#define BALL_H

#include <math.h>

#include <QTimer>
#include <QVector2D>
#include <QGraphicsPixmapItem>

//Esta clase modela la pelota que es utilizada durante el minijuego.

class Ball: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

    //El objetivo del minijuego será que el jugador suelte la pelota y esta describa un movimiento parabólico
    //mientras cae, además de ir rebotando contra las paredes con un coeficiente de restitución menor a la
    //unidad.

private:
    QTimer *move_timer;
    QList<QGraphicsItem*> collisions;

    //Puesto que el valor de Y aumenta hacia abajo de la pantalla, la componente en Y de la aceleración de
    //la gravedad será positiva.

    double initial_speed[2], initial_pos[2], restitution, time, scaled_gravity;
    short goal_pos[2], rebounds_num;

    void collision(short side);

    QRectF boundingRect() const;
    QPainterPath shape() const;

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
