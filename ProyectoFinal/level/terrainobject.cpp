#include "terrainobject.h"

TerrainObject::TerrainObject(short i, short j, short _type) : type(_type) {

    if (type == 1) {
        pix = new QPixmap(":/images/resources/images/rock.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 2) {
        pix = new QPixmap(":/images/resources/images/snail_fluid.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 3) {
        pix = new QPixmap(":/images/resources/images/fluid.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 4) pix = new QPixmap(":/images/resources/images/fluid.png");

    setPos(60*j, 60*i);
    setPixmap(*pix);
    setZValue(1);
}

TerrainObject::~TerrainObject() {
    delete pix;
}
