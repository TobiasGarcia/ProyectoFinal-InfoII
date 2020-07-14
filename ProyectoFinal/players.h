#ifndef PLAYERS_H
#define PLAYERS_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <array>

class Players: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    bool shot1, shot2;
    std::array<bool, 4> move_dir1, move_dir2;
    QTimer *move_timer, *delay_timer1, *delay_timer2;
    QPixmap *pix1, *pix2;

    void shoting(bool player);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QRectF boundingRect() const;

public:
    QGraphicsPixmapItem *multi;
    Players(short x1, short y1, short x2, short y2);
    ~Players();
public slots:
    void move();
    void ready_shot1() {shot1 = true;};
    void ready_shot2() {shot2 = true;};
signals:
    void add_fire_ball(short x, short y);
};

#endif // PLAYERS_H
