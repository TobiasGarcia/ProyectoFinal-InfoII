#ifndef TERRAINOBJECT_H
#define TERRAINOBJECT_H

#include <QGraphicsPixmapItem>

class TerrainObject: public QGraphicsPixmapItem {
private:
    short type;
    QPixmap *pix;
public:
    short get_type() {return type;};
    TerrainObject(short i, short j, short _type);
    ~TerrainObject() {delete pix;};
};

#endif // TERRAINOBJECT_H
