#ifndef SPECIAL_ENEMIES_H
#define SPECIAL_ENEMIES_H

#include "level/enemy.h"

//En esta libreria se definen el resto de clases que modelarán a los enemigos especiales del juego,
//todas heredan la clase Enemy.

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CARACOL-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//El caracol irá dejando un rastro de pegamento azul que le permitirá a los enemigos que caminen
//sobre el moverse más rápido.

class Snail: public Enemy {
private:
    TerrainObject *glue;

    void add_glue();
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Snail(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
        Enemy(i, j, 3, _level, _terrain, _list_index) {};
};

//----------------------------------------------------------------------------------------------------------------
//------------------------------------------------PUERCOESPÍN-----------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//El puercoespín irá directa hacia la base del jugdor, y si entra en contacto con alguna roca
//ambos se destruiran.

class Porcupine: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Porcupine(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
};

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------LECHUZA-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//La lechuza no es afectada por los efectos de los pegamentos ni las rocas, pues ella se mueve
//por el aire.

class Owl: public Enemy {
private:
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Owl(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
};

//----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------CAMALEÓN-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//El camaleón se hace invisible y visible cada cierto tiempo.

//La barra de vida lo delataría, por lo cual es invulnerable cuando está invisible, además,  si choca
//contra una roca, el camaleón se transporta a otra zona circundante y se hace invisible de nuevo,
//esto es para confundir al jugador y hacer de este un enemigo más interesante.

class Chamaleon: public Enemy {
private:
    bool visible;
    QTimer *camouflage_timer;

    void rock_collision();

public:
    Chamaleon(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Chamaleon();

public slots:
    void move();
    void camouflage();

};

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------TOPO--------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//El coloca un agujero cerca de la base y tiempo despues surge de el.

class Mole: public Enemy {
private:
    QVector2D tile_pixels;
    QGraphicsPixmapItem *hole;

public:
    QTimer *dig_timer;

    Mole(QGraphicsScene *_level, Terrain *_terrain, short _list_index);
    ~Mole();

public slots:
    void spawn();

};

//----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------BUITRE-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//El buitre entra al mapa y da una vuelta alrededor de la base, para luego ir directamente
//hacia ella y destruirla de un solo golpe. Como también se mueve por el aire, no es
//afectado por los pegamentos o las rocas.

class Vulture: public Enemy {
private:
    short a, b, loop_num;
    bool straight_line, strike;
    double angle, initial_angle, r, dr, c, offset;

    double radio();
    double diff_radio();
    bool collisions_handler(QList<QGraphicsItem*> collisions);

public:
    Vulture(QGraphicsScene *_level, Terrain *_terrain, short _list_index);

public slots:
    void move();

};

#endif // SPECIAL_ENEMIES_H
