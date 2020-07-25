#include "information.h"
#include <QDebug>

Information::Information(QGraphicsScene *_target_scene) : target_scene(_target_scene) {

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

    display_timer = new QTimer;
    display_timer->setSingleShot(true);
    connect(display_timer, &QTimer::timeout, this, &Information::remove);
}

Information::~Information() {
    delete text;
    delete display_timer;
}

void Information::set_display_time(unsigned long long int millis) {
    display_timer->start(millis);
}

void Information::display_message(short x, short y, QString message) {

    text->setPlainText(message);

    setRect(-10, -5, text->boundingRect().width() + 20, text->boundingRect().height() + 6);
    setPos(x - text->boundingRect().width()/2, y);

    //Por esto necesitamos target_scene, porque no podemos sobrescribir
    //el método scene() pues cuando no se tiene escena, se necesita
    //saber en cual hay que colocar la info.

    if (scene() == nullptr) target_scene->addItem(this);
}

void Information::remove() {

    //Es un slot de un QTimer.

    target_scene->removeItem(this);
}
