#include "roundrect.h"

void RoundRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (edge) {
        painter->drawRoundRect(QRect(0, 0, 40, 7), 5, 5);
    }
}

RoundRect::RoundRect(bool _edge) : edge(_edge) {
    if (edge) {
        setBrush(Qt::red);
        setPen(QPen(Qt::green, 0));
    }
}

QRectF RoundRect::boundingRect() const {
    return QRectF(0, 0, 40, 10);
}
