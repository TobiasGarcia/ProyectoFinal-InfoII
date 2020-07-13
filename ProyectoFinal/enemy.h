#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <math.h>
#include <QTimer>
#include <QQueue>
#include <QVector2D>
#include <QPainter>
#include <terrain.h>
#include <terrainobject.h>
#include <base.h>
#include <roundrect.h>
#include <QGraphicsScene>

class Enemy: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //Utilizamos QQueue porque necesitamos una estructura FIFO,
    //pues agregamos los targets y los vamos eliminando en orden
    //de llegada. Utilizamos QQueue en lugar de std::queue para
    //facilitar la interacción con los QVector2D.

    QGraphicsRectItem *health_bar;
    QGraphicsScene *level;
    Terrain *terrain;
    QQueue<QVector2D> targets;
    QVector2D dir, speed, speed_aux;
    QTimer *move_timer, *delay_timer, *health_on_timer;
    QPixmap *pix;
    short spd, width_half, height_half, type, health, max_health;
    QList<QGraphicsItem*> collisions;
    bool rotated, health_bar_on;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVector2D tiles2pixels(short i, short j);
    QVector2D tiles2pixels(QPoint point);
    void set_targets(short i, short j);
    bool entrance_exists(short side, short tile[2]);
    void middle_steps(short k, short probe[4]);
    QPoint sides2point(short side1, short side2);
    void hit(short tile[2]);
    void update_target();
    void initialize_health_bar();

    QPainterPath shape() const;

public:
    QTimer *bite_timer;
    //RoundRect *re;

    Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain);
    ~Enemy();

    void reduces_health();

public slots:
    void move();
    void finish_lapse();
    void health_off();
signals:
    void first_bite();
};

#endif // ENEMY_H
