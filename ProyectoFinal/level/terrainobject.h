#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include <QGraphicsPixmapItem>

//Esta clase modela los objetos que se pueden colocar en el mapa del juego.

class TerrainObject: public QGraphicsPixmapItem {
private:

    //La variable type indica el tipo de objeto al que corresponde esta instanciación de TerrainObject,
    //puede tomar los siguientes valores: 1 para una roca, 2 para un pegamento de caracol y 3 para
    //un pegamento colocado por los jugadores.

    short type;
    QPixmap *pix;

public:
    short get_type() {return type;};

    TerrainObject(short i, short j, short _type);
    ~TerrainObject();

};

#endif // TERRAINOBJECT_H
