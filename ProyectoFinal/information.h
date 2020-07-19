#ifndef INFORMATION_H
#define INFORMATION_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QFont>

class Information: public QGraphicsRectItem {
private:
    short width, height;
    QGraphicsScene *scene;
    QGraphicsTextItem *text;
public:
    Information(QGraphicsScene *_scene);
    ~Information();

    void display_message(short x, short y, QString message);
};

#endif // INFORMATION_H
