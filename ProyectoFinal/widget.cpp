#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {

    ui->setupUi(this);

    layout()->setMargin(0);
    setFixedSize(782, 602); //Dos pixeles de desface para que la escena quede a la medida.

    ui->game_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->game_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srand(time(nullptr));
    QFontDatabase::addApplicationFont(":/font/resources/kepler296.otf");

    path = "../ProyectoFinal/data/";
    two_players = false;

//    Game *game = new Game(path, true, "Johnny Cage");

//    Q_UNUSED(game);

//    level = new Level(true, path);
//    ui->graphicsView->setScene(level);

//    minigame = new Minigame(true);
//    ui->graphicsView->setScene(minigame);

//    levels_menu = new LevelsMenu(true);
//    ui->graphicsView->setScene(levels_menu);
}

Widget::~Widget() {
    //delete minigame;
    //delete levels_menu;
    //delete level;
    delete game;
    delete ui;
}


void Widget::on_new_btn_clicked() {
    ui->stacked_w->setCurrentIndex(1);
    ui->info_lb->setText("");
    ui->id_line->setText("");
    ui->pass_line->setText("");
    ui->pass2_line->setText("");
    ui->pass_cbox->setCheckState(Qt::Unchecked);
}

void Widget::on_return_new_btn_clicked() {
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_return_log_btn_clicked() {
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_load_btn_clicked() {
    ui->stacked_w->setCurrentIndex(2);
}

void Widget::on_pass_cbox_stateChanged(int arg1) {
    if (arg1) {
        ui->pass_line->setEchoMode(QLineEdit::Normal);
        ui->pass2_line->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->pass_line->setEchoMode(QLineEdit::Password);
        ui->pass2_line->setEchoMode(QLineEdit::Password);
    }
}

void Widget::on_register_btn_clicked() {

    unsigned short index;
    if ((ui->id_line->text() == "") or (ui->pass_line->text() == "") ) ui->info_lb->setText("Debes colocar un nombre y una\n"
                                                                                            "contraseña para tu partida.");
    else if (!normal_characters(ui->id_line->text())) ui->info_lb->setText("Lo sentimos, el nombre de la partida\n"
                                                                          "sólo puede tener números y letras sin\n"
                                                                          "tildes ni ñ (eñe).");
    else if (!normal_characters(ui->pass_line->text())) ui->info_lb->setText("Lo sentimos, la contraseña de la partida\n"
                                                                            "sólo puede tener números y letras sin\n"
                                                                            "tildes ni ñ (eñe).");
    else if (search_id(ui->id_line->text().toStdString(), index)) ui->info_lb->setText("Lo sentimos, esa partida ya existe,\n"
                                                                                       "ingresa otro nombre para poder crear\n"
                                                                                       "una nueva o si esa es tu partida, regresa\n"
                                                                                       "al menú principal para cargarla.");
    else if (ui->pass_line->text() != ui->pass2_line->text()) ui->info_lb->setText("Las contraseñas no coinciden");
    else start_new_game();
}

bool Widget::normal_characters(QString str) {

    short ascii;
    for (short i = 0; i < str.length(); i++) {
        ascii = short(str[i].toLatin1());
        if (((ascii < 48) or (57 < ascii)) and ((ascii < 65) or (90 < ascii)) and ((ascii < 97) or (122 < ascii))){
            return false;
        }
    }
    return true;
}

bool Widget::search_id(std::string id, unsigned short &index) {

    //-----------------------------------Para este momento ya hemos verificado que el archivo y users.txt exista

    //Utilizamos unsigned short porque es muy poco probable que hayan más de
    //65535 partidas guardadas.

    index = 0;
    bool exists = false;
    if (games_id_pass.empty()) {

        //Es probable que hayan entre 0 y 8 partidas, por lo cual nos anticipamos para reservar
        //memoria, pero de todas formas si hay más de 8 partidas el contenedor vector simplemente
        //se realocará para almacenar las demás.

        games_id_pass.reserve(8);

        short id_len;
        std::string data;
        std::array<std::string, 2> id_pass;
        std::fstream file(path + "users.txt", std::ios::in);

        while (getline(file, data)) {

            id_len = data.find(':');
            id_pass[0] = data.substr(0, id_len);
            id_pass[1] = data.substr(id_len + 1);
            games_id_pass.push_back(id_pass);

            if (!exists and (id_pass[0] == id)) {
                exists = true;
                index = (games_id_pass.size() - 1);
            }
            qDebug() << QString::fromUtf8(data.c_str());
        }
        file.close();
    }
    else {
        short len = games_id_pass.size();
        while (!exists and (index < len)) {
            if (games_id_pass[index][0] == id) exists = true;
            else index++;
        }
    }
    return exists;
}

void Widget::start_new_game() {

    //El registro en el archivo .txt lo hace la clase game cuando inicializa
    //las variables de la partida y el arhivo con estos datos.

    std::array<std::string, 2> id_pass;
    id_pass[0] = ui->id_line->text().toStdString();
    id_pass[1] = ui->pass_line->text().toStdString();
    games_id_pass.push_back(id_pass);

    ui->stacked_w->setCurrentIndex(3);
    game = new Game(path, ui->game_gv, two_players, id_pass[0], id_pass[1]);
}

void Widget::on_mode_btn_clicked() {
    if (two_players) ui->mode_btn->setText("Un solo jugador");
    else ui->mode_btn->setText("Dos jugadores");
    two_players = !two_players;
}
































































































