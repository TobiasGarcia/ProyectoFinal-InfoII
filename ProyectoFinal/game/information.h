#ifndef INFORMATION_H
#define INFORMATION_H

#include <QPen>
#include <QFont>
#include <QBrush>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

//Esta clase es utilizada para colocarle en pantalla mensajes el usuario con un
//aspecto visual conforme al juego.

class Information: public QObject,  public QGraphicsRectItem {

    Q_OBJECT

private:
    QTimer *display_timer;
    QGraphicsTextItem *text;
    QGraphicsPixmapItem *pix;
    QGraphicsScene *target_scene;

    short width, height;

public:
    Information(QGraphicsScene *_target_scene);
    ~Information();

    void display_message(short x, short y, QString message);
    void display_message(short x, short y, QString message, QString image_path);

    void set_display_time(unsigned long long int millis);

public slots:
    void remove();

};

#endif // INFORMATION_H
