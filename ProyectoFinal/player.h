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

    QRectF boundingRect() const;

public:
    QTimer *delay_timer;
    std::array<bool, 4> move_dir;

    Player(short i, short j, bool _player_one = true);
    ~Player();

public slots:
    void move();
};

#endif // PLAYER_H
