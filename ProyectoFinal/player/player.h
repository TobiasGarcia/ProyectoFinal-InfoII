#ifndef PLAYER_H
#define PLAYER_H

#include <QTimer>
#include <QGraphicsPixmapItem>

//Esta clase modelará a los jugadores.

class Player: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

    //Para el movimiento de los jugadores utilizamos un arreglo de bools llamado move_dir, el cual indica
    //cual de las flechas del teclado, o cual de las letras WASD para el segundo jugador, están siendo
    //presionadas por el usuario, para de esta forma poder mover al objeto Player en esa dirección.
    //La correspondencia entre los índices del arreglo y las direcciones es la siguiente: 0 arriba,
    //1 izquierda, 2 abajo y 3 derecha.

private:
    QPixmap *pix;
    QTimer *move_timer;

    short lower_limit;
    bool player_one;

    QRectF boundingRect() const;

public:

    //La variable freez indica si los jugadores se pueden mover o no, false para cuando no se pueden mover,
    //o true para cuando sí. La coloamos como variable pública pues debemos leerla y modificarla desde
    //fuera de la clase Player.

    bool freez;

    //Utitilizamos un arreglo pues las direcciones de movimiento simepre son 4.

    std::array<bool, 4> move_dir;

    Player(short i, short j, bool lower_limit, bool _player_one = true);
    ~Player();

public slots:
    void move();

};

#endif // PLAYER_H
