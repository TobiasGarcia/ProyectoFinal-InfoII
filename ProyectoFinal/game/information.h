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
    QGraphicsScene *target_scene;
    QGraphicsTextItem *text;
    QGraphicsPixmapItem *pix;
public:
    Information(QGraphicsScene *_target_scene);
    ~Information();

    void set_display_time(unsigned long long int millis);
    void display_message(short x, short y, QString message);
    void display_message(short x, short y, QString message, QString image_path);
public slots:
    void remove();
};

#endif // INFORMATION_H
