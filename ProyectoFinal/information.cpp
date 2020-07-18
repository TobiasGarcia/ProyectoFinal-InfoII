#include "information.h"
#include <QDebug>

Information::Information(short x, short y, QString message) {

    text = new QGraphicsTextItem(this);

    text->setPlainText(message);
    text->setDefaultTextColor(Qt::white);
    text->setFont(QFont("a Black Lives", 16));

    width = text->boundingRect().width();
    height = text->boundingRect().height();

    setBrush(QColor(104, 109, 117));
    setPen(QPen(QColor(224, 162, 43), 5));

    setRect(-5, -5, width + 10, height + 6);
    setPos(x - width/2, y);
    setZValue(7);
}

Information::~Information() {
    delete text;
}
