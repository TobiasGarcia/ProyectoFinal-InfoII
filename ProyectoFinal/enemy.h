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

    QVector2D dir;
    QTimer *health_on_timer;
    QPixmap *pix;
    short spd, width_half, height_half, type, health, max_health;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVector2D tiles2pixels(QPoint point);
    bool entrance_exists(short side, short tile[2]);
    void middle_steps(short k, short probe[4]);
    QPoint sides2point(short side1, short side2);
    void initialize_health_bar();
    void initialize();
    QPainterPath shape() const;

protected:
    QQueue<QVector2D> targets;
    QGraphicsRectItem *health_bar;
    bool rotated, health_bar_on;
    QVector2D speed, speed_aux;
    QTimer *move_timer, *delay_timer;
    QGraphicsScene *level;
    Terrain *terrain;

    void update_target();
    void recalculate_initial_tile(short tile[2]);
    void set_targets(short i, short j);
    QVector2D tiles2pixels(short i, short j);

    virtual    void rock_collision();
    virtual bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    QTimer *bite_timer;
    //RoundRect *re;

    Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain);
    ~Enemy();

    void reduces_health();

public slots:
    void move();
    void finish_delay();
    void health_off();
signals:
    void first_bite();
};

#endif // ENEMY_H
