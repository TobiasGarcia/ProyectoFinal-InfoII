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

    //level = new Level;
    //ui->graphicsView->setScene(level);

    minigame = new Minigame;
    ui->graphicsView->setScene(minigame);
}

Widget::~Widget() {
    delete minigame;
    //delete level;
    delete ui;
}

