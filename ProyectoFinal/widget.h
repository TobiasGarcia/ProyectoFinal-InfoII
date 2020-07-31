#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include <fstream>

#include <QWidget>

#include "game/game.h"
#include "level/level.h"
#include "minigame/minigame.h"
#include "levels_menu/levelsmenu.h"

//NOTA SOBRE COMPATIBILIDAD: Este proyecto fue desarrollado y se comprobó su buen funcionamiento bajo el
//sistema operativo Windows 10. Las teclas utilizadas son las que Qt detecta como las siguientes:

//Letras: W, A, S, D, V, B, P.
//Las cuatro flechas del teclado.
//La tecla Enter cercana a las flechas del teclado.
//La tecla Esapcio.
//La tecla para borrar, la que se encuentra arriba de la tecla Enter.
//La tecla Shift Derecho.
//La tecla Esc.

//NOTA SOBRE LA DOCUMENTACIÓN: La mayor parte de la documentación se encuentra dentro de los .cpp,
//aunque también hay muchos comentarios en los .h.

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
    void on_mode_btn_clicked();
    void on_register_btn_clicked();
    void on_return_new_btn_clicked();
    void on_pass_cbox_stateChanged(int arg1);

    void on_load_btn_clicked();
    void on_load_game_btn_clicked();
    void on_return_log_btn_clicked();
    void on_pass_log_cbox_stateChanged(int arg1);

    void exit_game();
    void on_exit_btn_clicked();

private:
    Ui::Widget *ui;
    QString style_single, style_multi, style_sheet;

    Game *game;

    bool two_players;
    std::string path;

    //Toda la información sobre usuarios y contraseñas se almacenará en el vector games_id_pass,
    //esto es con el propósito de no tener que consultar el archivo users.txt frecuentemente.
    //Cada elemento es un arreglo de dos strings, un usuario y su respectiva contraseña.

    //Utilizamos un vector pues en principio no sabemos cuantos usuarios hay registrados, además de
    //que necesitamos un rápido acceso a estos datos para buscar información de logueo.

    std::vector<std::array<std::string, 2>> games_id_pass;

    void initialice_styles();
    void lines_edit_styles();
    void check_boxs_styles();
    void buttons_styles();
    void labels_styles();


    void load_game();
    void start_new_game();

    bool valid_characters(QString str);
    bool search_id(std::string id, unsigned short &index);
    bool check_txt_files(bool &game_data_exists, std::string game_id = "");
};
#endif // WIDGET_H
