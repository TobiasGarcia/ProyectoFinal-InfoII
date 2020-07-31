#ifndef NEEDLE_H
#define NEEDLE_H

#include <math.h>

#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTimer>

//Esta clase modela la aguja de la brújula del menú de niveles.

class Needle: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

    //Dentro del menú para escoger los niveles, habrá una brújula cuya aguja oscilará describiendo un movimiento
    //armónico simple (MAS), y que cuando el jugador seleccione un nivel, esta comenzará a describir una
    //oscilación subamortiguada hasta terminar apuntando hacia el nivel seleccionado.

    //NOTA IMPORTANTE: Este sistema NO describirá el movimiento de un péndulo, pues el movimiento pendular no es
    //un MAS, resulta muy aproximado a este cuando el ángulo es pequeño, pero aquí utilizaremos angulos entre
    //0 y 2*pi radianes, por lo cual la aguja no describirá un movimiento pendular, sino que simplemente
    //utilizaremos los modelos del MAS y la oscilación amortiguada para modificar su posición angular
    //y darle movimiento.

private:
    QPixmap *pix;
    QTimer *move_timer;

    //La variable ang_frecuency almacenará la frecuencia angular del MAS; la variable time nos ayudará a
    //llevar el registro del tiempo transcurrodo desde el inicio del movimiento, aunque mediante la variable
    //period, cuando el valor de time supere el período de oscilación del MAS, le restaremos este mismo valor
    //para evitar que la variable time aumente indefinidamente; la variable amp almacenará la amplitud del
    //movimiento; la variable gamma corresponde a un parámetro que utiliza el modelo de la oscilación
    //subamortiguada; la variables phase nos será de utilidad para realizar la transición entre el
    //MAS y la oscilació subamortiguada; finalmente la variable offset nos ayudará a trasladar el
    //punto de equilibrio de las oscilaciones, para que la aguja pueda apuntar en diferentes
    //direcciones.

    double ang_frecuency, time, gamma, phase, amp, period, offset;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    Needle();
    ~Needle();

    void point_direction(short direction);

public slots:
    void move();

signals:
    void finish();
};

#endif // NEEDLE_H
