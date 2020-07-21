#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <math.h>
#include <QTimer>
#include <QQueue>
#include <QVector2D>
#include <QPainter>
#include "level/terrain.h"
#include "level/terrainobject.h"
#include "level/base.h"
#include <QGraphicsScene>

class Enemy: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //Utilizamos QQueue porque necesitamos una estructura FIFO,
    //pues agregamos los targets y los vamos eliminando en orden
    //de llegada. Utilizamos QQueue en lugar de std::queue para
    //facilitar la interacción con los QVector2D.

    //La variable defeated es para poder hacer que la barra de vida del enemigo
    //aparezca un momento más justo después de que el enemigo fue derrotado, esto
    //es una forma de indicarle a los jugadores que han acabado con el enemigo.
    //Parece algo redundante, pero en realidad resulta útil cuando los
    //jugadores golpean a un enemigo invisible mediante un power up,
    //además de que sin esta opción parecía que los enemigos
    //simplemente desaparecían.

    bool defeated;
    QVector2D dir;
    QTimer *health_on_timer;
    QPixmap *pix;
    short width_half, height_half, type, health, max_health;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVector2D tiles2pixels(QPoint point);
    bool entrance_exists(short side, short tile[2]);
    void middle_steps(short k, short probe[4]);
    QPoint sides2point(short side1, short side2);
    void initialize_health_bar();
    void initialize();
    QPainterPath shape() const;

protected:
    QQueue<QVector2D> targets;
    QGraphicsRectItem *health_bar;
    bool rotated, health_bar_on, freez;
    QVector2D speed, speed_aux;
    QTimer *move_timer, *delay_timer;
    //Necesitamos la varible level para tener un puntero a la escena y poder modificarla
    //desde dentro de los métodos del enemigo, prescindimos de utilizar la función scene()
    //como en la clase fireball porque presentaba problemas al utilizarla dentro de los
    //enemigos especiales.
    QGraphicsScene *level;
    Terrain *terrain;
    short list_index, spd;

    void update_target();
    void recalculate_initial_tile(short tile[2]);
    QVector2D tiles2pixels(short i, short j);

    virtual void set_targets(short i, short j);
    virtual void rock_collision();
    virtual bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    QTimer *bite_timer;

    short get_type() {return type;};
    void set_freez(bool _freez) {freez = _freez;};
    Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Enemy();

    void reduces_health(short hit);

public slots:
    virtual void move();
    void finish_delay();
    void health_off();
    void update_index(short removed_index);

signals:
    void first_bite();
    void remove_enemy(short list_index);
};

#endif // ENEMY_H
