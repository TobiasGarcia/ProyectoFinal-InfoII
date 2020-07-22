#ifndef POWERUP_H
#define POWERUP_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <math.h>
#include <QPainter>

class PowerUp: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    QTimer *move_timer;
    QPixmap *pix;
    short spd, path_type, power_type;;
    bool straight_line, freez;

    //Parametros de las curvas. Tipo float para que
    //no dividan de forma entera, doube no para no
    //gastarle 8 bytes.
    float a, b;

    //Esas últimas dos variables, c y s, con utilizadas en el cálculo de la derivada
    //del astroide, resultan útiles pues en la fórmula aparece muchas veces las funciones
    //seno y coseno, por lo cual calculamos sus valores sobre el ángulo actual para sálo
    //tener que realizar lecturas de estos valores en lugar de recalcularlas cada vez.
    double angle = 0, r, dr, m_angle, period, c, s;

    void initialize();
    double radio();
    double diff_radio();
    QPainterPath shape() const;

public:
    void emit_give_power();
    PowerUp(short _path_type, short _power_type);
    ~PowerUp();
    void set_freez(bool _freez) {freez = _freez;};

public slots:
    void move();
    void lemniscate_move();
signals:
    void give_power(short power_type);
};

#endif // POWERUP_H
