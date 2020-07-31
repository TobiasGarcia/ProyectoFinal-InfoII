#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {

    ui->setupUi(this);

    //Nuestra escena es de 780 x 600 pixeles, por lo cual el Widget debe ser de 782 x 602
    //para compensar el pixel extra (uno en cada dirección) de los bordes de la ventana.

    layout()->setMargin(0);
    setFixedSize(782, 602);
    setWindowTitle("Galaxy Lacters");

    //Retiramos las scroll bars del QGraphicsView.

    ui->game_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->game_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    initialice_styles();

    //Todos los txt con información que necesita el juego se encuentran en la carpeta data,
    //por lo cual en la variable path debe ir la ruta hasta esta con un '/' al final.

    //La primer ruta es para el modo Debug, mientras que la segunda es utilizada cuando
    //el proyecto es compilado de modo Release.

    path = "../ProyectoFinal/data/";
    //path = "data/";

    srand(time(nullptr));
    two_players = false;
    game = nullptr;
}

Widget::~Widget() {
    if (game != nullptr) delete game;
    delete ui;
}

void Widget::initialice_styles() {

    //Este método se encarga de agregar los estilos para los botones, para los labels, para las líneas de texto,
    //para los check boxs y colocar las imágenes de fondo de las páginas del QStackedWidget. Es sólo para el
    //aspecto visual.

    QPalette palette;
    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/main_menu.png"));
    ui->stacked_w->widget(0)->setAutoFillBackground(true);
    ui->stacked_w->widget(0)->setPalette(palette);

    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/new_game.png"));
    ui->stacked_w->widget(1)->setAutoFillBackground(true);
    ui->stacked_w->widget(1)->setPalette(palette);

    palette.setBrush(QPalette::Background, QPixmap(":/graphic_interface/resources/images/graphic_interface/load_game.png"));
    ui->stacked_w->widget(2)->setAutoFillBackground(true);
    ui->stacked_w->widget(2)->setPalette(palette);

    //Agregamos una nueva font desde la carpeta de recursos para los estilos que utilizaremos.

    QFontDatabase::addApplicationFont(":/font/resources/kepler296.otf");

    labels_styles();
    buttons_styles();
    lines_edit_styles();
    check_boxs_styles();
}

void Widget::labels_styles() {

    style_sheet = "font: large \"Kepler 296\";"
                  "color: white;";

    ui->id_lb->setStyleSheet(style_sheet);
    ui->pass_lb->setStyleSheet(style_sheet);
    ui->pass2_lb->setStyleSheet(style_sheet);
    ui->info_lb->setStyleSheet(style_sheet);
    ui->mode_lb->setStyleSheet(style_sheet);

    ui->id_log_lb->setStyleSheet(style_sheet);
    ui->pass_log_lb->setStyleSheet(style_sheet);
    ui->info_log_lb->setStyleSheet(style_sheet);
}

void Widget::lines_edit_styles() {

    style_sheet = "font: large \"Kepler 296\";"
                  "font-size: 22px;"
                  "color: #ecaf39;";

    ui->id_line->setStyleSheet(style_sheet);
    ui->pass_line->setStyleSheet(style_sheet);
    ui->pass2_line->setStyleSheet(style_sheet);

    ui->id_log_line->setStyleSheet(style_sheet);
    ui->pass_log_line->setStyleSheet(style_sheet);
}

void Widget::buttons_styles() {

    //Las QStrings style_single y style_multi corresponden a los estilos del botón que alterna entre el modo
    //de un solo jugador y multijugador.

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

    style_sheet = ":active {background-color: #b14533; border: 3px solid #853021; border-radius: 10px;"
                  "         font: large \"Kepler 296\";"
                  "         font-size: 22px;"
                  "         color: #69271b;}"
                  ":hover {background-color: #b74d3c; border: 3px solid #8d392b;"
                  "        color: #712e22;}";

    ui->new_btn->setStyleSheet(style_sheet);
    ui->load_btn->setStyleSheet(style_sheet);
    ui->exit_btn->setStyleSheet(style_sheet);

    ui->return_new_btn->setStyleSheet(style_sheet);
    ui->register_btn->setStyleSheet(style_sheet);
    ui->mode_btn->setStyleSheet(style_single);

    ui->return_log_btn->setStyleSheet(style_sheet);
    ui->load_game_btn->setStyleSheet(style_sheet);
}

void Widget::check_boxs_styles() {

    style_sheet = "font: large \"Kepler 296\";"
                  "color: #ecaf39;";

    ui->pass_cbox->setStyleSheet(style_sheet);
    ui->pass_log_cbox->setStyleSheet(style_sheet);
}

void Widget::on_new_btn_clicked() {

    //Si vamos a crear un juego nuevo, reiniciamos todos los campos
    //necesarios para el registro.

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

    //Si vamos a cargar la partida reiniciamos todos los campos necesarios.

    ui->info_log_lb->setText("");
    ui->id_log_line->setText("");
    ui->pass_log_line->setText("");
    ui->stacked_w->setCurrentIndex(2);
    ui->pass_log_cbox->setCheckState(Qt::Unchecked);
}

void Widget::on_pass_cbox_stateChanged(int arg1) {

    //Alternamos entre modo normal y modo contraseña las QLineEdit.

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

    //Alternamos entre modo normal y modo contraseña la QLineEdit.

    if (arg1) ui->pass_log_line->setEchoMode(QLineEdit::Normal);
    else ui->pass_log_line->setEchoMode(QLineEdit::Password);
}

void Widget::on_mode_btn_clicked() {

    //Alternamos entre la opción de un jugaodr y multijugador dentro de la interfaz para crear una nueva
    //partida, además de reflejar el cambio en el estilo del botón.

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

void Widget::exit_game() {

    //Este slot se ejecuta cuando el usuario sale del juego al menú principal de la interfaz
    //gráfica, pero aún la aplicación no se ha cerrado.

    delete game;
    game = nullptr;
    ui->stacked_w->setCurrentIndex(0);
}

void Widget::on_exit_btn_clicked() {
    QCoreApplication::quit();
}

bool Widget::valid_characters(QString str) {

    //Retornamos true si la string ingresada sólo posee letras, minúsculas o mayúsculas, (a excepción de la ñ),
    //o números; false en caso contrario.

    short ascii;
    for (short i = 0; i < str.length(); i++) {
        ascii = short(str[i].toLatin1());
        if (((ascii < 48) or (57 < ascii)) and ((ascii < 65) or (90 < ascii)) and ((ascii < 97) or (122 < ascii))){
            return false;
        }
    }
    return true;
}

bool Widget::check_txt_files(bool &game_data_exists, std::string game_id) {

    //Este método es utilizado para verificar que los archivos .txt con la información necesaria para el juego
    //se encuentren donde deben estar. Retornamos true si todo está en orden, o false si falta por lo menos
    //uno de ellos. La variable game_data_exists indica si la partida ya está registrada o no, en caso de
    //que inicialmente se encuentre en true procedemos a verificar la existencia de los datos de la
    //partida, y si estos existen, le dejamos el valor de true, o en caso contrario, le colocamos
    //false; si inicialmente se encontraba en false, es porque se está creando una partida nueva,
    //por lo cual sus datos aún no existen y por tanto no los buscamos y le dejamos el valor
    //de false a game_data_exists.

    //Las rutas de los archivos de interés, desde la carpeta data, son las siguientes:

    //data/users.txt       -- La información de acceso de las partidas.

    //data/games_data/     -- Múltiples archivos .txt, uno por cada partida guardada.
    //                        Llevan el nombre de su correspondiente partida, ejemplo manuel.txt.
    //                        Estos archivos almacenan la información del estado de la partida.

    //data/levels_scripts/ -- Múltiples archivos .txt, dos por cada nivel. Estos archivos almacenan
    //                        las instrucciónes que ejecuta cada nivel, como colocar un enemigo,
    //                        esperar cierto tiempo, iniciar una oleada, terminar el nivel, etc.
    //                        esto se explica más detalladamente dentro de la documenttación de
    //                        la clase Level.
    //                        El primer número del nombre (1 o 0) indica si son las instrucciones
    //                        para el modo multijugador o el de un solo jugador respectivamente,
    //                        luego sigue la palabra level y por último el número del
    //                        nivel (de 1 a 3), ejemplo: 0level2.txt.

    //data/levels_dialogs/ -- Múltiples archivos .txt, uno por cada nivel. Estos archivos almacenan
    //                        los diálogos que se le presentarán al usuario al inicio de cada nivel,
    //                        para más información consultar la documentación de la clase Level.
    //                        Llevan de nombre la palabra level seguida del númeor del nivel (de 0 a 3),
    //                        ejemplo: level3.txt.

    std::fstream file(path + "users.txt");

    if (!file.is_open()) return false;
    else file.close();

    for (short i = 0; i < 4; i++) {

        file.open(path + "levels_dialogs/level" + char(i + 48) + ".txt");
        if (!file.is_open()) return false;
        else file.close();

        //El condicional es necesario pues no existen los archivos 0level0.txt ni 1level.txt.

        if (i != 0) {
            file.open(path + "levels_scripts/0level" + char(i + 48) + ".txt");
            if (!file.is_open()) return false;
            else file.close();

            file.open(path + "levels_scripts/1level" + char(i + 48) + ".txt");
            if (!file.is_open()) return false;
            else file.close();
        }
    }

    //Como mencionamos antes, si game_data_exists es true, es porque debemos verificar la existencia
    //de los datos de la partida dentro de la carpeta data/games_data; si es false, no debemos
    //hacer nada al respecto.

    if (game_data_exists) {

        //Actualizamos el valor de game_data_exists, pues esta variable es recibida por referencia,
        //para indicar si efectivamente los datos existen o no.

        file.open(path + "games_data/" + game_id + ".txt");
        if (!file.is_open()) game_data_exists = false;
        else {
            game_data_exists = true;
            file.close();
        }
    }

    return true;
}

bool Widget::search_id(std::string id, unsigned short &index) {

    //Este método retorna true si la id ingresada ya está registrada dentro del archivo users.txt, además de que
    //carga toda la información de este en el contenedor games_id_pass; en caso de que esta ya se encuentre cargada
    //buscará la información dentro de games_id_pass para no tener que volver a consultar users.txt. En caso de
    //retornar true en la variable index almacenará el índice del usuario dentro del vector games_id_pass; si
    //la id no se encuentra registrada retornamos false y dejamos index en 0.

    //Para index basta usar unsigned short porque es muy poco probable que hayan más
    //de 2^16 = 65536 partidas guardadas (no está de más recordar que no hace falta
    //restarle 1 al 2^16 pues es número total de partidas, no el máximo índice,
    //el cual sí corresponde a 65535).

    index = 0;
    bool exists = false;

    //Si luego del siguiente proceso exists continua siendo false, es porque la partida no existe.

    //Como mencinamos antes, la información de users.txt se cargará en games_id_pass, por lo cual
    //si la información aún no ha sido cargada, lo hacemos, o en caso contrario siemplemente
    //buscamos la id en games_id_pass.

    if (games_id_pass.empty()) {

        //Como es probable que hayan entre 0 y 8 partidas, nos anticipamos y reservamos memoria
        //para 8 elementos del vector games_id_pass, de esta forma evitamos algunos realocamientos
        //de este, sin embargo, si se superan las 8 partidas no habrá problema, solo que el vector
        //comenzará a realocarse por potencias de 2 como normalmente hace.

        games_id_pass.reserve(8);

        short id_len;
        std::string data;
        std::array<std::string, 2> id_pass;

        //Cuando se llame a este método ya se habrá chequeado la existencia de los archivos .txt
        //dentro del método check_txt_files(), por lo cual podemos abrir el archivo tranquilamente.

        std::fstream file(path + "users.txt", std::ios::in);

        while (getline(file, data)) {

            //El formato de users.txt es:

            //PARTIDA:CONTRASEÑA\n

            //Donde PARTIDA es el nombre de la partida y CONTRASEÑA la respectiva contraseña.

            id_len = data.find(':');
            id_pass[0] = data.substr(0, id_len);
            id_pass[1] = data.substr(id_len + 1);
            games_id_pass.push_back(id_pass);

            //Recordemos que este método posee dos propósitos, cargar la información y buscar la id,
            //por lo cual cada vez que agregamos un elementos revisamos si corresponde con la id de
            //interés, esto es siempre que no la hayamos encontrado aún, es decir, exists todavía
            //se encuentra en false.

            if (!exists and (id_pass[0] == id)) {
                exists = true;
                index = (games_id_pass.size() - 1);
            }
        }
        file.close();
    }
    else {

        //En caso de que la información ya haya sido cargada previamente por este método,
        //simplemente buscamos la id dentro de games_id_pass.

        short len = games_id_pass.size();
        while (!exists and (index < len)) {
            if (games_id_pass[index][0] == id) exists = true;
            else index++;
        }
    }
    return exists;
}

void Widget::on_register_btn_clicked() {

    //Este método se encarga de verificar todas las condiciones requeridas respecto a los campos necesarios
    //antes de realizar un registro nuevo.

    //Inicializamos game_data_exists en false pues estamos creando una partida nueva, por lo cual sus
    //datos aún no existen.

    unsigned short index;
    bool game_data_exists = false;
    if ((ui->id_line->text() == "") or (ui->pass_line->text() == "") ) ui->info_lb->setText("Debes colocar un nombre y una\n"
                                                                                            "contraseña para tu partida.");
    else if (!valid_characters(ui->id_line->text())) ui->info_lb->setText("Lo sentimos, el nombre de la\n"
                                                                          "partida sólo puede contener\n"
                                                                          "números, letras distintas de\n"
                                                                          "la ñ (eñe) y sin acentos.");
    else if (!valid_characters(ui->pass_line->text())) ui->info_lb->setText("Lo sentimos, la contraseña de la\n"
                                                                            "partida sólo puede contener\n"
                                                                            "números, letras distintas de\n"
                                                                            "la ñ (eñe) y sin acentos.");
    else if (!check_txt_files(game_data_exists)) ui->info_lb->setText("Lo sentimos pero en este\n"
                                                                      "momento no podemos iniciar\n"
                                                                      "el juego pues faltan archivos\n"
                                                                      "en la carpeta data/, por favor\n"
                                                                      "consultar al desarrollador.");
    else if (search_id(ui->id_line->text().toStdString(), index)) ui->info_lb->setText("Lo sentimos, esa partida ya existe,\n"
                                                                                       "ingresa otro nombre para poder\n"
                                                                                       "crear una nueva o si esa es tu\n"
                                                                                       "partida, regresa al menú principal\n"
                                                                                       "para cargarla.");
    else if (ui->pass_line->text() != ui->pass2_line->text()) ui->info_lb->setText("Las contraseñas no coinciden");
    else start_new_game();
}

void Widget::start_new_game() {

    //Este método se encarga de crear y comenzar la nueva partida.

    //La creación .txt dentro de data/games_data y el registro de la partida dentro de users.txt
    //es realizado por la clase Game, mientras que la actualización de la información de games_id_pass
    //es realizada dentro de este método, pues Game no tiene acceso a games_id_pass debido a que está
    //diseñado para sólo maneja la información de una partida a la vez, mientras que games_id_pass
    //maneja la información de acceso de todas.

    std::array<std::string, 2> id_pass;
    id_pass[0] = ui->id_line->text().toStdString();
    id_pass[1] = ui->pass_line->text().toStdString();
    games_id_pass.push_back(id_pass);

    //Colocamos la cuarta página (index 3) del QStackedWidget y creamos el juego. La clase Game ya se encarga
    //de comenzar el juego.

    ui->stacked_w->setCurrentIndex(3);
    game = new Game(path, ui->game_gv, id_pass[0], id_pass[1], two_players);
    connect(game, &Game::exit_game, this, &Widget::exit_game);
}

void Widget::on_load_game_btn_clicked() {

    //Este método se encarga de verificar todas las condiciones requeridas respecto a los campos necesarios
    //antes de cargar una partida.

    //Inicializamos game_data_exists en true pues necesitamos verificar si los datos de la partida
    //a cargar existen.

    unsigned short index;
    bool game_data_exists = true;
    if ((ui->id_log_line->text() == "") or (ui->pass_log_line->text() == "") ) ui->info_log_lb->setText("Debes colocar el nombre y la\n"
                                                                                                        "contraseña de tu partida.");
    else if (!check_txt_files(game_data_exists, ui->id_log_line->text().toStdString())) ui->info_log_lb->setText("Lo sentimos pero en este\n"
                                                                                                                 "momento no podemos iniciar\n"
                                                                                                                 "el juego pues faltan archivos\n"
                                                                                                                 "en la carpeta data/, por favor\n"
                                                                                                                 "consultar al desarrollador.");
    else if (!search_id(ui->id_log_line->text().toStdString(), index)) ui->info_log_lb->setText("Lo sentimos, esa partida no existe,\n"
                                                                                                "si deseas crear una partida nueva\n"
                                                                                                "lo puedes hacer desde el menú\n"
                                                                                                "principal presionando el botón\n"
                                                                                                "juego nuevo.");

    //Si no salimos por el anterior else if, es porque la partida está registrada dentro de users.txt,
    //pero aún existe la posibilidad de que no existan sus datos dentro de la carpeta data/games_data,
    //por lo cual consultamos game_data_exists pues en esta variable se encuentra almacenada la
    //información sobre la existencia de estos datos, luego de haber pasado por el método
    //check_txt_files().

    else if (!game_data_exists) ui->info_log_lb->setText("Lo sentimos, el archivo con la\n"
                                                         "infromación de la partida no se\n"
                                                         "encuentra dentro de la carpeta\n"
                                                         "data/games_data/, por favor\n"
                                                         "consultar al desarrollador.");
    else if (ui->pass_log_line->text().toStdString() != games_id_pass[index][1]) ui->info_log_lb->setText("El nombre o la contraseña de\n"
                                                                                                          "la partida es incorrecto.");
    else load_game();
}

void Widget::load_game() {

    //Colocamos la cuarta página (index 3) y dejamos que la clase Game cargue toda la información
    //de la partida.

    ui->stacked_w->setCurrentIndex(3);
    game = new Game(path, ui->game_gv, ui->id_log_line->text().toStdString());
    connect(game, &Game::exit_game, this, &Widget::exit_game);
}
