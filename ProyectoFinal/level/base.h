#ifndef BASE_H
#define BASE_H

#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

//Esta clase modela la base que los jugadores deben defender para superar el nivel.

class Base: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QPixmap *pix;
    QTimer *hurt_timer;
    QGraphicsRectItem *health_bar;
    QGraphicsPixmapItem *lifebuoy;

    bool vulnerable, *extra_life, tutorial_level;
    short *health;

public:
    QGraphicsPixmapItem *center;

    Base(QGraphicsRectItem *_health_bar, short *_health, bool *_extra_life,
         QGraphicsPixmapItem *_lifebuoy, bool _tutorial_level);
    ~Base();

    short get_health() {return (*health);};

    void increase_health(short increment);

    void set_vulnerable(bool _vulnerable) {vulnerable = _vulnerable;};

public slots:
    void bitten();
    void vulture_hit();
    void return_normal();

signals:
    void no_health();

};

#endif // BASE_H
