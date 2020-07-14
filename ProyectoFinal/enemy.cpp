#include "enemy.h"
#include <QDebug>

Enemy::Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain) :
    type(_type), level(_level), terrain(_terrain) {

    initialize();

    setZValue(1);

    setPixmap(*pix);
    setPos(tiles2pixels(i, j).toPoint());

    health = max_health;
    health_bar_on = false;

    initialize_health_bar();

    set_targets(i, j);
    update_target();
    rotated = false;

    bite_timer = new QTimer;

    health_on_timer = new QTimer;
    health_on_timer->setSingleShot(true);
    connect(health_on_timer, &QTimer::timeout, this, &Enemy::health_off);

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Enemy::move);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Enemy::finish_delay);

    move_timer->start(50);
}

Enemy::~Enemy() {
    delete pix;
    delete delay_timer;
    delete move_timer;
    delete bite_timer;
    delete health_on_timer;
    delete[] health_bar;
}

void Enemy::reduces_health() {

    health -= 100;
    if (health == 0) delete this;
    else {

        (health_bar + 1)->setRect(1, 1, 38*(health/float(max_health)), 5);
        if (!health_bar_on) {
            health_bar_on = true;
            health_bar->setPos(x() - 20, y() - 35);
            level->addItem(health_bar);
        }
        health_on_timer->start(1000);
    }
}

void Enemy::move() {

    speed = speed_aux;
    if (collisions_handler(collidingItems(Qt::IntersectsItemBoundingRect))) return;

    //Le damos un "empujonsito" aumentado su rapidez para poder
    //evitar que los enemigos más lentos se atasquen al girar
    //por una esquina.
    if (rotated) speed = 1.6*speed;
    setPos(x() + speed[0]*0.1, y() + speed[1]*0.1);
    if (health_bar_on) health_bar->setPos(health_bar->x() + speed[0]*0.1, health_bar->y() + speed[1]*0.1);

    dir = targets.head() - QVector2D(pos());
    rotated = false;

    if (dir.length() < 7) {
        targets.dequeue();
        //En caso de problemas descomentar lo de abajo
        //y borrar las dos lineas siguientes.
        //-----------------
        update_target();
        rotated = true;
        //-----------------
//        if (targets.empty()) move_timer->stop();
//        else {
//            update_target();
//            rotated = true;
//        }
    }
}

void Enemy::finish_delay() {
    update_target();
    move_timer->start(50);
}

void Enemy::health_off() {
    health_bar_on = false;
    level->removeItem(health_bar);
}

QRectF Enemy::boundingRect() const {
    return QRectF(-width_half, -height_half, width_half*2, height_half*2);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Esto es para que se vea con buena resolución al realizar rotaciones.
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(-width_half, -height_half, *pix);
}

QVector2D Enemy::tiles2pixels(short i, short j) {
    //Lo colocamos en la mitad del cuadrado, por eso el 29.
    return QVector2D(29 + 60*j, 29 + 60*i);
}

//Función sobrecargada
QVector2D Enemy::tiles2pixels(QPoint point) {
    return QVector2D(29 + 60*point.y(), 29 + 60*point.x());
}

void Enemy::set_targets(short i, short j) {

    short gap[4] = {1, 0, -1, 0}, tile[2], probe[4];

    //En probe almacenamos el orden de las direcciones en que vamos
    //a sondear las paredes para buscar una entrada.

    if ((i == -1) or (i == 9)) {
        probe[0] = (i + 1)/5;
        probe[1] = (j<=6)?1:3;
    }
    else {
        probe[0] = (j + 8)/7;
        probe[1] = (i<4)?0:2;
    }

    probe[2] = (probe[1] + 2)%4;
    probe[3] = (probe[0] + 2)%4;

    //Según donde encontremos una entrada, utilziamos gap para terminar
    //de ajustar la forma en que el enemigo va a entrar.

    for (short k = 0; k < 4; k++) {
        if (entrance_exists(probe[k], tile)) {
            middle_steps(k, probe);
            targets.enqueue(tiles2pixels(tile[0] - gap[probe[k]], tile[1] + gap[(probe[k] + 1)%4]));
            targets.enqueue(tiles2pixels(tile[0] + gap[probe[k]], tile[1] - gap[(probe[k] + 1)%4]));
            targets.enqueue(tiles2pixels(4, 6));
            return;
        }
    }
}

QPoint Enemy::sides2point(short side1, short side2) {

    //El producto de sus sucesores define una relación del conjunto
    //de parejas de números al de puntos de la matriz tiles tal que
    //se distinguen las parejas por los números, independientemente
    //del orden en que se encuentren.

    short product = (side1 + 1)*(side2 + 1);

    if (product == 2) return QPoint(1, 2);
    else if (product == 4) return QPoint(1, 10);
    else if (product == 6) return QPoint(7, 2);
    else return QPoint(7, 10);
}

void Enemy::recalculate_initial_tile(short tile[2]) {

    //Aproxima la posición a una de salida.

    short i = short(y()/60), j = short(x()/60);

    if ((i == 1) or (i == 7)) {
        tile[0] = (5*i - 8)/3;
        tile[1] = j;
    }
    else {
        tile[0] = i;
        tile[1] = (7*j - 18)/4;
    }
}

void Enemy::update_target() {
    dir = targets.head() - QVector2D(pos());
    speed = spd*dir.normalized();
    speed_aux = speed;
    setRotation(90 - (atan2(-dir[1], dir[0])*180/M_PI));
}

void Enemy::initialize_health_bar() {

    //Cuando se elimina la escena se deletea rect, por lo cual
    //no lo podemos colocar en el destructor de Enemy.
    health_bar = new QGraphicsRectItem[2];
    health_bar->setBrush(QColor(86, 86, 86));
    health_bar->setPen(QColor(66, 66, 66));
    health_bar->setRect(0, 0, 40, 7);

    (health_bar + 1)->setParentItem(health_bar);
    (health_bar + 1)->setBrush(QColor(54, 104, 195));
    (health_bar + 1)->setPen(QColor(54, 104, 195));
    (health_bar + 1)->setRect(1, 1, 38, 5);
}

void Enemy::initialize() {
    if (type == 0) {
        //36 x 36 pixeles;
        width_half = 18;
        height_half = 18;
        spd = 30;
        max_health = 400;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy0.png");
    }
    else if (type == 1) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 20;
        max_health = 600;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy1.png");
    }
    else if (type == 2) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 20;
        max_health = 600;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy1.png");
    }
    else if (type == 3) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 35;
        max_health = 400;
        pix = new QPixmap(":/enemies/resources/images/enemies/snail.png");
    }
    else if (type == 4) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 40;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/porcupine.png");
    }
    else if (type == 5) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 25;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/owl.png");
    }
    else if (type == 6) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 20;
        max_health = 500;
        pix = new QPixmap(":/enemies/resources/images/enemies/chamaleon.png");
    }
    else if (type == 7) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 20;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/mole.png");
    }
    else {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 140;
        max_health = 500;
        pix = new QPixmap(":/enemies/resources/images/enemies/vulture.png");
    }
}

void Enemy::rock_collision() {

    setPos(x() - speed[0]*0.2, y() - speed[1]*0.2);
    if (health_bar_on) health_bar->setPos(health_bar->x() - speed[0]*0.2, health_bar->y() - speed[1]*0.2);

    short tile[2];
    targets.clear();
    recalculate_initial_tile(tile);
    set_targets(tile[0], tile[1]);

    move_timer->stop();
    delay_timer->start(300);
}

bool Enemy::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Retornamos true si necesitamos dejar de ejecutar el slot move().

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {

            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);
            if ((terrain_object->get_type() == 1) and !rotated) {
                rock_collision();
                return true;
            }
            else if (terrain_object->get_type() == 2) speed = 1.4*speed_aux;
            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
        }
        else if (typeid(*item) == typeid(Base)) {
            move_timer->stop();

            //No se puede emitir la señal timeout manualmente,
            //por lo cual definimos una señal auxiliar para
            //enviar al comenzar el timer.
            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

//Colocar formas más acertadas.

QPainterPath Enemy::shape() const {
    QPainterPath path;
    if (type == 0) path.addEllipse(QRect(7-width_half, 1-height_half, 22, 34));
    else if (type == 1) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 2) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 3) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 4) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 5) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 6) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 7) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    return path;
}

void Enemy::middle_steps(short k, short probe[4]) {
    if (k == 0) return;
    else if (k == 1) targets.enqueue(tiles2pixels(sides2point(probe[0], probe[1])));
    else if (k == 2) targets.enqueue(tiles2pixels(sides2point(probe[0], probe[2])));
    else {
        targets.enqueue(tiles2pixels(sides2point(probe[0], probe[1])));
        targets.enqueue(tiles2pixels(sides2point(probe[1], probe[3])));
    }
}

bool Enemy::entrance_exists(short side, short tile[2]) {

    //Buscamos si hay entradas en el lado side: 0, 1, 2, 3 <-> up, left, down, right.
    //En caso afirmativo retornamos true, o false en caso contrario.

    short index;
    QVector<short> entrances_index;
    //Serán máximo 5 entradas, por lo cual reservamos cinco elementos
    //para evitar realocamientos.
    entrances_index.reserve(5);

    if (side%2) {

        //Vamos por arriba o por abajo

        index = 3*side;
        for (short k = 0; k < 3; k++) {
            if (terrain->tiles[k + 3][index] != 1) entrances_index.push_back(k + 3);
        }

        //Si no hay entradas retornamos falso, o en caso contrario
        //la almacenamos en tile.

        if (entrances_index.isEmpty()) return false;
        else {
            tile[0] = entrances_index[rand()%entrances_index.size()];
            tile[1] = index;
            return true;
        }
    }
    else {

        //Vamos por los costados.

        index = 2*side + 2;
        for (short k = 0; k < 5; k++) {
            if (terrain->tiles[index][k + 4] != 1) entrances_index.push_back(k + 4);
        }

        if (entrances_index.isEmpty()) return false;
        else {
            tile[0] = index;
            tile[1] = entrances_index[rand()%entrances_index.size()];
            return true;
        }
    }
}

//    if ((i == -1) or (i == 9)) {
//        if (entrance_exists(0, terrain, tile)) {
//            targets.enqueue(tiles2pixels(tile[0] - 1, tile[1]));
//            targets.enqueue(tiles2pixels(tile[0] + 1, tile[1]));
//        }
//        else if (entrance_exists(1, terrain, tile)) {
//            targets.enqueue(tiles2pixels(tile[0], tile[1] - 1));
//            targets.enqueue(tiles2pixels(tile[0], tile[1] + 1));
//        }
//        else if (entrance_exists(3, terrain, tile)) {
//            targets.enqueue(tiles2pixels(tile[0], tile[1] + 1));
//            targets.enqueue(tiles2pixels(tile[0], tile[1] - 1));
//        }
//        else {
//            entrance_exists(3, terrain, tile);
//            targets.enqueue(tiles2pixels(tile[0] + 1, tile[1]));
//            targets.enqueue(tiles2pixels(tile[0] - 1, tile[1]));
//        }
//    }

//    if (i == -1) {
//        for (short j = 4; j < 9; j++) {
//            if (terrain->tiles[2][j] != 1) entrances_index.push_back(j);
//        }
//        index = entrances_index[rand()%entrances_index.size()];
//        targets.enqueue(tiles2pixels(1, index));
//        targets.enqueue(tiles2pixels(3, index));
//    }
//    else if (i == 9) {
//        for (short j = 4; j < 9; j++) {
//            if (terrain->tiles[6][j] != 1) entrances_index.push_back(j);
//        }
//        index = entrances_index[rand()%entrances_index.size()];
//        targets.enqueue(tiles2pixels(7, index));
//        targets.enqueue(tiles2pixels(5, index));
//    }
//    else if (j == -1) {
//        for (short i = 3; i < 6; i++) {
//            if (terrain->tiles[i][3] != 1) entrances_index.push_back(i);
//        }
//        index = entrances_index[rand()%entrances_index.size()];
//        targets.enqueue(tiles2pixels(index, 2));
//        targets.enqueue(tiles2pixels(index, 4));
//    }
//    else {
//        for (short i = 3; i < 6; i++) {
//            if (terrain->tiles[i][9] != 1) entrances_index.push_back(i);
//        }
//        index = entrances_index[rand()%entrances_index.size()];
//        targets.enqueue(tiles2pixels(index, 10));
//        targets.enqueue(tiles2pixels(index, 8));
//    }

//    targets.enqueue(tiles2pixels(4, 6));

//NOTA: Dentro de los siguientes cuatro condicionales el código es muy similar,
//salvo algunos índices y el orden de los .enqueue(), por lo cual cuando traté de
//escribir un solo código y utilizar los condicionales sólo para definir los
//parámetros con los que este se debía ejecutar, noté que se volvía muy
//abstracto y difícil de entender para otra persona que lo estuviese
//leyendo, además de que no hacía una diferencia sustancial mayor a
//ahorrar unas cuantas líneas, de donde preferí darle preferencia
//a la legibilidad del código antes que a su extensión.

































































