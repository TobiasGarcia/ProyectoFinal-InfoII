#include "terrainobject.h"

TerrainObject::TerrainObject(short x, short y, short _type) : type(_type) {

    if (type == 1) {
        pix = new QPixmap(":/images/resources/images/roca.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 3) {
        pix = new QPixmap(":/images/resources/images/fluido.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    setPixmap(*pix);
    setPos(x, y);
    setZValue(0);
}
