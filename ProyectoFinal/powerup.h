#ifndef POWERUP_H
#define POWERUP_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <math.h>

class PowerUp: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QTimer *move_timer;
    QPixmap *pix;
    short spd, path_type, power_type;;
    bool straight_line;

    //Parametros de las curvas. Tipo float para que
    //no dividan de forma entera, doube no para no
    //gastarle 8 bytes.
    float a, b;
    double angle = 0, r, dr, m_angle;

    double radio();
    double diff_radio();

public:
    PowerUp(short _path_type, short _power_type);
    ~PowerUp();

public slots:
    void move();
    void lemniscate_move();
};

#endif // POWERUP_H
