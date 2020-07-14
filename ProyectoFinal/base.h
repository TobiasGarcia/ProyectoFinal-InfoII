#ifndef BASE_H
#define BASE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Base: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QGraphicsRectItem *health_bar;
    QPixmap *pix;
    short health;
    QTimer *bitten_timer;

public:
    Base(QGraphicsRectItem *_health_bar);
    ~Base();
    QGraphicsPixmapItem *center;
public slots:
    void bitten();
    void return_normal();
};

#endif // BASE_H
