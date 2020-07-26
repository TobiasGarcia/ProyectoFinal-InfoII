#ifndef NEEDLE_H
#define NEEDLE_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <math.h>
#include <QPainter>

class Needle: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    double ang_frecuency, time, gamma, phase, amp, T, offset;
    QPixmap *pix;
    QTimer *move_timer;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    Needle();
    ~Needle();

    void point_direction(short direction);
public slots:
    void move();
signals:
    void finish();
};

#endif // NEEDLE_H
