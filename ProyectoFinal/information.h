#ifndef INFORMATION_H
#define INFORMATION_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>

class Information: public QGraphicsRectItem {
private:
    short width, height;
    QGraphicsTextItem *text;
public:
    Information(short x, short y, QString message);
    ~Information();
};

#endif // INFORMATION_H
