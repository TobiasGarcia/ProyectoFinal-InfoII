#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {

    ui->setupUi(this);

    layout()->setMargin(0);
    setFixedSize(782, 602); //Dos pixeles de desface para que la escena quede a la medida.

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //srand(time(nullptr));
    //QFontDatabase::addApplicationFont(":/font/resources/kepler296.otf");

//    std::string path = "../ProyectoFinal/data/";

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
    //delete ui;
}

