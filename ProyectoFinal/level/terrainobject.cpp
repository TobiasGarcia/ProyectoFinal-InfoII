#include "terrainobject.h"

TerrainObject::TerrainObject(short i, short j, short _type) : type(_type) {

    //Colocamos la imágen correspondiente de acuerdo al valor de la variable _type; las variables
    //i y j indican la casilla del mapa donde se debe colocar el objeto.

    if (type == 1) {
        pix = new QPixmap(":/levels/resources/images/levels/rock.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 2) {
        pix = new QPixmap(":/levels/resources/images/levels/snail_glue.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }
    else if (type == 3) {
        pix = new QPixmap(":/levels/resources/images/levels/glue.png");
        *pix = pix->transformed(QTransform().translate(-30, -30).rotate(90*(rand()%4)).translate(30, 30));
    }

    setPos(60*j, 60*i);
    setPixmap(*pix);
    setZValue(1);
}

TerrainObject::~TerrainObject() {
    delete pix;
}
