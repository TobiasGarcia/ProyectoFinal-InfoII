#ifndef PLAYERS_H
#define PLAYERS_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <array>

class Players: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    std::array<bool, 4> move_dir1, move_dir2;
    QTimer *move_timer;
    QPixmap *pix1, *pix2;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
public:
    QGraphicsPixmapItem *multi;
    Players(short x1, short y1, short x2, short y2);
    ~Players();
public slots:
    void move();
};

#endif // PLAYERS_H
