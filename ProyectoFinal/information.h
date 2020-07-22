#ifndef INFORMATION_H
#define INFORMATION_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QTimer>

class Information: public QObject,  public QGraphicsRectItem {

    Q_OBJECT

private:
    QTimer *display_timer;
    short width, height;
    QGraphicsScene *scene;
    QGraphicsTextItem *text;
public:
    Information(QGraphicsScene *_scene);
    ~Information();

    void set_display_time(short millis);
    void display_message(short x, short y, QString message);
public slots:
    void remove();
};

#endif // INFORMATION_H
