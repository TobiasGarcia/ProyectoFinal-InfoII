#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    layout()->setMargin(0);
    setFixedSize(797, 602); //Dos pixeles de desface para que la escena quede a la medida.

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 794, 599); //795x600 pixeles para que los jugadores se muevan de 15 en 15.
    ui->graphicsView->setScene(scene);

    players = new Players(390, 285, 495, 285); //Colocarlos en múltiplos de 15.
    players->setFlag(QGraphicsItem::ItemIsFocusable);
    players->setFocus();
    scene->addItem(players);
    scene->addItem(players->multi);
}

Widget::~Widget()
{
    delete players;
    delete scene;
    delete ui;
}

