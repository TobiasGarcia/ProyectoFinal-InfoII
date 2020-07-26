#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Player: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QPixmap *pix;
    QTimer *move_timer;
    bool player_one;
    short lower_limit;

    QRectF boundingRect() const;

public:
    QTimer *delay_timer;
    bool freez;
    std::array<bool, 4> move_dir;

    Player(short i, short j, bool lower_limit, bool _player_one = true);
    ~Player();

public slots:
    void move();
};

#endif // PLAYER_H
