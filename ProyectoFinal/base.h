#ifndef BASE_H
#define BASE_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Base: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    bool vulnerable;
    QGraphicsRectItem *health_bar;
    QPixmap *pix;
    short health;
    QTimer *bitten_timer;

public:
    Base(QGraphicsRectItem *_health_bar);
    ~Base();
    QGraphicsPixmapItem *center;
    void set_vulnerable(bool _vulnerable) {vulnerable = _vulnerable;};
public slots:
    void bitten();
    void return_normal();
};

#endif // BASE_H
