#ifndef BLACKSCREEN_H
#define BLACKSCREEN_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QTimer>

class BlackScreen: public QObject, public QGraphicsRectItem {

    Q_OBJECT

private:
    short opacity;
    bool increase_opacity;
    QTimer *opacity_timer;
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
