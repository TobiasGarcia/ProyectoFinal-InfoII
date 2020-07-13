#include "terrainobject.h"

TerrainObject::TerrainObject(short x, short y, short _type) : type(_type) {

    if (type == 1) {
        pix = new QPixmap(":/images/resources/images/rock.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 3) {
        pix = new QPixmap(":/images/resources/images/fluid.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 4) pix = new QPixmap(":/images/resources/images/fluid.png");

    setPixmap(*pix);
    setPos(x, y);
    setZValue(0);
}
