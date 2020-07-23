#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "level/level.h"
#include "minigame/minigame.h"
#include "levels_menu/levelsmenu.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {

    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Level *level;
    Minigame *minigame;
    LevelsMenu *levels_menu;
};
#endif // WIDGET_H
