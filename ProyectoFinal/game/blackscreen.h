#ifndef BLACKSCREEN_H
#define BLACKSCREEN_H

#include <QBrush>
#include <QTimer>
#include <QGraphicsRectItem>

//Esta clase es utilizada para colocar una pantalla negra sobre la escena,
//además de utilizar métodos para cambiar su opacidad gradualmente
//generando un efecto visual para las transiciones entre las
//escenas.

class BlackScreen: public QObject, public QGraphicsRectItem {

    Q_OBJECT

private:
    QTimer *opacity_timer;

    bool increase_opacity;
    short opacity;

public:
    BlackScreen();
    ~BlackScreen();

    void change_opacity(bool _increase_opacity);

public slots:
    void update_opacity();

signals:
    void finish();

};

#endif // BLACKSCREEN_H
