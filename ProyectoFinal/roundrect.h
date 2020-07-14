#ifndef ROUNDRECT_H
#define ROUNDRECT_H

#include <QGraphicsRectItem>
#include <QPainter>

class RoundRect: public QGraphicsRectItem {
private:
    bool edge;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    RoundRect(bool _edge);
};

#endif // ROUNDRECT_H
