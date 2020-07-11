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

class Enemy: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //Utilizamos QQueue porque necesitamos una estructura FIFO,
    //pues agregamos los targets y los vamos eliminando en orden
    //de llegada. Utilizamos QQueue en lugar de std::queue para
    //facilitar la interacción con los QVector2D.

    Terrain *terrain;
    QQueue<QVector2D> targets;
    QVector2D dir, speed, speed_aux;
    QTimer *move_timer, *lapse_timer;
    QPixmap *pix;
    short spd, width_half, height_half;
    QList<QGraphicsItem*> collisions;
    bool rotated;

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

public:
    Enemy(short i, short j, Terrain *_terrain);
    ~Enemy();

public slots:
    void move();
    void finish_lapse();
};

#endif // ENEMY_H
