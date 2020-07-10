#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <math.h>
#include <QTimer>
#include <QVector2D>
#include <QPainter>

class Enemy: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    //Utilizamos list para evitar el realocamiento del contenedor
    //vector, pues no necesitamos rapidez en acceso. El QList es
    //para que interactue bien con los QPoints.
    QList<QVector2D> targets;
    QVector2D dir, speed;
    QTimer *move_timer;
    QPixmap *pix;
    short spd, width_half, height_half;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    Enemy();
    ~Enemy();

public slots:
    void move();
};

#endif // ENEMY_H
