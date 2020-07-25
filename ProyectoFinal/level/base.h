#ifndef BASE_H
#define BASE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QTimer>

class Base: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QGraphicsPixmapItem *lifebuoy;
    bool vulnerable, *extra_life;
    QGraphicsRectItem *health_bar;
    QPixmap *pix;
    short *health;
    QTimer *hurt_timer;

public:
    Base(QGraphicsRectItem *_health_bar, short *_health, bool *_extra_life, QGraphicsPixmapItem *_lifebuoy);
    ~Base();
    QGraphicsPixmapItem *center;
    void increase_health(short increment);
    void set_vulnerable(bool _vulnerable) {vulnerable = _vulnerable;};
    short get_health() {return (*health);};

public slots:
    void bitten();
    void return_normal();
    void vulture_hit();

signals:
    void no_health();
};

#endif // BASE_H
