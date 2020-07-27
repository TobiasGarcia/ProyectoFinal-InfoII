#ifndef ENEMY_H
#define ENEMY_H

#include <math.h>
#include <QTimer>
#include <QQueue>
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "level/terrainobject.h"
#include "level/terrain.h"
#include "level/base.h"


class Enemy: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //La variable dir almacena la dirección en la que el enemigo se está moviendo, en este caso no utilizamos
    //la noción de vector como simples arreglos de dos elemetnos, sino como una forma de identificar una
    //dirección junto con una magnitud.

    QVector2D dir;

    bool defeated;
    QTimer *health_on_timer;

    QPixmap *pix;
    short width_half, height_half, type, health;

    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QVector2D tiles2pixels(QPoint point);
    QPoint sides2point(short side1, short side2);

    bool entrance_exists(short side, short tile[2]);
    void middle_steps(short k, short probe[4]);

    void define_personality();
    void initialize_timers();
    void initialize_health_bar();

protected:

    //La sección de protected es para los atributos que se utilizan dentro de métodos de
    //las clases que heredan Enemy.

    //Para el movimiento de los enemigos utilizamos un sistema de objetivos (targets), los cuales no son más
    //que objetos de clase QVector2D, a los cuales el enemigo se va a ir dirigiendo en línea recta, y cuando
    //alcanza el primero, lo eliminamos del contenedor y dirigimos al enemigo hacia el siguiente. Como se
    //puede concluir de inmediato, esto describe una estructura FIFO, por lo cual el contender apropiado
    //es una queue, pero para facilitar la compatibilidad con los QVector2D utilizamos QQueue.

    QQueue<QVector2D> targets;
    QVector2D speed, speed_aux;

    QTimer *move_timer, *delay_timer;
    QGraphicsRectItem *health_bar;
    bool health_bar_on, freez;

    Terrain *terrain;
    QGraphicsScene *level;
    short list_index, spd, max_health;

    void update_target();
    QVector2D tiles2pixels(short i, short j);
    void recalculate_initial_tile(short tile[2]);

    //Estos tres métodos son declarados utilizando la palabra reservada virtual para poder reimplementarlos
    //dentro de las clases que heredan Enemy.

    virtual void rock_collision();
    virtual void set_targets(short i, short j);
    virtual bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    QTimer *bite_timer;

    void reduces_health(short hit);
    short get_type() {return type;};
    void set_freez(bool _freez) {freez = _freez;};

    Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Enemy();

public slots:

    //El siguiente es declarado utilizando la palabra reservada virtual para poder reimplementarlo
    //dentro de una de las clases que heredan Enemy.

    virtual void move();

    void health_off();
    void finish_delay();
    void update_index(short removed_index);

signals:
    void first_bite();
    void remove_enemy(short list_index);

    //Desde una de las clases que heredan Enemy, la clase Vulture, necesitamos emitir una señal, por lo cual
    //se requiere utilizar la macro Q_OBJECT en la declaración de la clase, sin embargo, el compilador
    //presentaba problemas al colocar la macro, por lo cual la alternativa que se tomó fue declarar
    //la señal dentro de la clase Enemy como tal, aunque esta sólo sea utilizada por la clase
    //Vulture que hereda Enemy.

    void vulture_hit();
};

#endif // ENEMY_H
