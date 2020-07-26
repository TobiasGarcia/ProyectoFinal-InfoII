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

    QPalette palette, palette2;
    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/main_menu.png"));
    ui->stacked_w->widget(0)->setAutoFillBackground(true);
    ui->stacked_w->widget(0)->setPalette(palette);

    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/new_game.png"));
    ui->stacked_w->widget(1)->setAutoFillBackground(true);
    ui->stacked_w->widget(1)->setPalette(palette);

    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/load_game.png"));
    ui->stacked_w->widget(2)->setAutoFillBackground(true);
    ui->stacked_w->widget(2)->setPalette(palette);

    style_single = ":active {background-color: #ecbb5d; border: 5px solid #e0a22b; border-radius: 10px;"
                   "         font: large \"Kepler 296\";"
                   "         font-size: 22px;"
                   "         color: #222c8d;}"
                   ":hover {background-color: #e9be6d; border: 5px solid #e6a935;"
                   "        color: #2a3494;}";

    style_multi = ":active {background-color: #ecbb5d; border: 5px solid #e0a22b; border-radius: 10px;"
                  "         font: large \"Kepler 296\";"
                  "         font-size: 22px;"
                  "         color: #58197d;}"
                  ":hover {background-color: #e9be6d; border: 5px solid #e6a935;"
                  "        color: #642589;}";

    QString style_sheet = ":active {background-color: #b14533; border: 3px solid #853021; border-radius: 10px;"
                          "         font: large \"Kepler 296\";"
                          "         font-size: 22px;"
                          "         color: #69271b;}"
                          ":hover {background-color: #b74d3c; border: 3px solid #8d392b;"
                          "        color: #712e22;}";

    QFontDatabase::addApplicationFont(":/font/resources/kepler296.otf");

    ui->new_btn->setStyleSheet(style_sheet);
    ui->load_btn->setStyleSheet(style_sheet);
    ui->exit_btn->setStyleSheet(style_sheet);

    ui->return_new_btn->setStyleSheet(style_sheet);
    ui->register_btn->setStyleSheet(style_sheet);

    ui->return_log_btn->setStyleSheet(style_sheet);
    ui->load_game_btn->setStyleSheet(style_sheet);

    style_sheet = "font: large \"Kepler 296\";"
                  "color: white;";

    ui->id_lb->setStyleSheet(style_sheet);
    ui->pass_lb->setStyleSheet(style_sheet);
    ui->pass2_lb->setStyleSheet(style_sheet);

    ui->id_log_lb->setStyleSheet(style_sheet);
    ui->pass_log_lb->setStyleSheet(style_sheet);

    style_sheet = "font: large \"Kepler 296\";"
                  "font-size: 22px;"
                  "color: #ecaf39;";

    ui->id_line->setStyleSheet(style_sheet);
    ui->pass_line->setStyleSheet(style_sheet);
    ui->pass2_line->setStyleSheet(style_sheet);

    ui->id_log_line->setStyleSheet(style_sheet);
    ui->pass_log_line->setStyleSheet(style_sheet);

    style_sheet = "font: large \"Kepler 296\";"
                  "color: #ecaf39;";

    ui->pass_cbox->setStyleSheet(style_sheet);
    ui->pass_log_cbox->setStyleSheet(style_sheet);

    style_sheet = "font: large \"Kepler 296\";"
                  "color: white;";

    ui->info_lb->setStyleSheet(style_sheet);
    ui->info_log_lb->setStyleSheet(style_sheet);

    ui->mode_btn->setStyleSheet(style_single);

    path = "../ProyectoFinal/data/";
    srand(time(nullptr));
    two_players = false;

    game = nullptr;

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
    if (game != nullptr) delete game;
    delete ui;
}


void Widget::on_new_btn_clicked() {
    two_players = false;
    ui->info_lb->setText("");
    ui->id_line->setText("");
    ui->pass_line->setText("");
    ui->pass2_line->setText("");
    ui->stacked_w->setCurrentIndex(1);
    ui->mode_btn->setText("Un solo Jugador");
    ui->mode_btn->setStyleSheet(style_single);
    ui->pass_cbox->setCheckState(Qt::Unchecked);
}

void Widget::on_return_new_btn_clicked() {
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_return_log_btn_clicked() {
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_load_btn_clicked() {
    ui->info_log_lb->setText("");
    ui->id_log_line->setText("");
    ui->pass_log_line->setText("");
    ui->stacked_w->setCurrentIndex(2);
    ui->pass_log_cbox->setCheckState(Qt::Unchecked);
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

void Widget::on_pass_log_cbox_stateChanged(int arg1) {
    if (arg1) ui->pass_log_line->setEchoMode(QLineEdit::Normal);
    else ui->pass_log_line->setEchoMode(QLineEdit::Password);
}

void Widget::exit_game() {
    delete game;
    game = nullptr;
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_register_btn_clicked() {

    unsigned short index;
    if ((ui->id_line->text() == "") or (ui->pass_line->text() == "") ) ui->info_lb->setText("Debes colocar un nombre y una\n"
                                                                                            "contraseña para tu partida.");
    else if (!normal_characters(ui->id_line->text())) ui->info_lb->setText("Lo sentimos, el nombre de la\n"
                                                                           "partida sólo puede contener\n"
                                                                           "números, letras distintas de\n"
                                                                           "la ñ (eñe) y sin acentos.");
    else if (!normal_characters(ui->pass_line->text())) ui->info_lb->setText("Lo sentimos, la contraseña de la\n"
                                                                             "partida sólo puede contener\n"
                                                                             "números, letras distintas de\n"
                                                                             "la ñ (eñe) y sin acentos.");
    else if (search_id(ui->id_line->text().toStdString(), index)) ui->info_lb->setText("Lo sentimos, esa partida ya existe,\n"
                                                                                       "ingresa otro nombre para poder\n"
                                                                                       "crear una nueva o si esa es tu\n"
                                                                                       "partida, regresa al menú principal\n"
                                                                                       "para cargarla.");
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
    game = new Game(path, ui->game_gv, id_pass[0], id_pass[1], two_players);
    connect(game, &Game::exit_game, this, &Widget::exit_game);
}

void Widget::on_mode_btn_clicked() {
    if (two_players) {
        ui->mode_btn->setText("Un solo Jugador");
        ui->mode_btn->setStyleSheet(style_single);
    }
    else {
        ui->mode_btn->setText("Dos Jugadores");
        ui->mode_btn->setStyleSheet(style_multi);
    }
    two_players = !two_players;
}

void Widget::on_exit_btn_clicked() {
    QCoreApplication::quit();
}

void Widget::on_load_game_btn_clicked() {
    unsigned short index;
    if ((ui->id_log_line->text() == "") or (ui->pass_log_line->text() == "") ) ui->info_log_lb->setText("Debes colocar el nombre y la\n"
                                                                                                        "contraseña de tu partida.");
    else if (!search_id(ui->id_log_line->text().toStdString(), index)) ui->info_log_lb->setText("Lo sentimos, esa partida no existe,\n"
                                                                                                "si deseas crear una partida nueva\n"
                                                                                                "lo puedes hacer desde el menú\n"
                                                                                                "principal presionando el botón\n"
                                                                                                "juego nuevo.");
    else if (ui->pass_log_line->text().toStdString() != games_id_pass[index][1]) ui->info_log_lb->setText("El nombre o la contraseña de\n"
                                                                                                          "la partida es incorrecto.");
    else load_game();
}

void Widget::load_game() {
    ui->stacked_w->setCurrentIndex(3);
    game = new Game(path, ui->game_gv, ui->id_log_line->text().toStdString());
    connect(game, &Game::exit_game, this, &Widget::exit_game);
}
































































































