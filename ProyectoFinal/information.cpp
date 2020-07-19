#include "information.h"
#include <QDebug>

Information::Information(QGraphicsScene *_scene) : scene(_scene) {

    text = new QGraphicsTextItem(this);

    text->setDefaultTextColor(Qt::white);
    //text->setFont(QFont("a Black Lives", 16));
    text->setFont(QFont("Kepler 296", 16));

    //El método setPlainText() de la clase QGraphicsTextItem tarda mucho en ejecutarse la primera
    //vez que es invocado, por lo cual colocamos un texto cualesquiera para que cuando se llame
    //al método display_message() este no tarde tanto en colocar el texto.

    text->setPlainText("Default");

    setBrush(QColor(104, 109, 117));
    setPen(QPen(QColor(224, 162, 43), 5));

    setZValue(7);
}

Information::~Information() {
    delete text;
}

void Information::display_message(short x, short y, QString message) {

    text->setPlainText(message);

    setRect(-10, -5, text->boundingRect().width() + 20, text->boundingRect().height() + 6);
    setPos(x - text->boundingRect().width()/2, y);

    scene->addItem(this);
}
