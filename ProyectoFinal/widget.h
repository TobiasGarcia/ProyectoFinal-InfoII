#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "level/level.h"
#include "minigame/minigame.h"
#include "levels_menu/levelsmenu.h"
#include "game.h"
#include <fstream>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {

    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_new_btn_clicked();

    void on_return_new_btn_clicked();

    void on_return_log_btn_clicked();

    void on_load_btn_clicked();

    void on_pass_cbox_stateChanged(int arg1);

    void on_register_btn_clicked();

    void on_mode_btn_clicked();

    void on_exit_btn_clicked();

    void on_load_game_btn_clicked();

    void on_pass_log_cbox_stateChanged(int arg1);

    void exit_game();

private:
    QString style_single, style_multi;
    bool two_players;
    Ui::Widget *ui;
    Level *level;
    Minigame *minigame;
    LevelsMenu *levels_menu;
    std::string path;
    Game *game;

    //Mantenemos la información del control de acceso cargada.

    std::vector<std::array<std::string, 2>> games_id_pass;

    bool normal_characters(QString str);
    bool search_id(std::string id, unsigned short &index);
    void start_new_game();
    void load_game();
};
#endif // WIDGET_H
