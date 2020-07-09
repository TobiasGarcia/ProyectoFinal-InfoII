#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <players.h>

class Level: public QGraphicsScene {
private:
    Players *players;
public:
    Level();
    ~Level();
};

#endif // LEVEL_H
