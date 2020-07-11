#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <players.h>
#include <terrain.h>
#include <terrainobject.h>
#include <enemy.h>

class Level: public QGraphicsScene {
private:
    Enemy *carlos;
    Terrain *terrain;
    Players *players;
public:
    Level();
    ~Level();
    void display_terrain();
};

#endif // LEVEL_H
