#include "fireball.h"

FireBall::FireBall(short x, short y) {

    //Las variables x e y indican la posición de la escena donde se debe colocar la fire ball.

    //El propósito de la variable erase es explicado dentro de la documentación del método
    //update() de esta misma clase.

    erase = false;

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/players/resources/images/players/fire_ball0.png");
    pix[1] = QPixmap(":/players/resources/images/players/fire_ball1.png");

    //Colocamos la fire ball en la posición indicada.

    setPos(x - 17.5, y - 17.5);
    setPixmap(pix[0]);
    setZValue(4);

    //Construimos el objeto update_timer, lo conectamos y llamamos su método start() para
    //comenzar con el ciclo de vida de la fire ball.

    update_timer = new QTimer;
    connect(update_timer, &QTimer::timeout, this, &FireBall::update);
    update_timer->start(150);
}

FireBall::~FireBall() {
    delete[] pix;
    delete update_timer;
}

void FireBall::update() {

    //Cada fire ball cuenta con una animación de dos imágenes, este método se encarga de
    //colocar la segunda imágen o de eliminar la fire ball.

    //Utilizamos la variable erase para saber que cuando se encuentre en true, es porque
    //ya habíamos colocado la segunda imágen y es hora de eliminar el objeto.

    if (!erase) {
        setPixmap(pix[1]);
        erase = true;
    }
    else delete this;
}

void FireBall::test_collisions() {

    //Este método procesa las colisiones de la fire ball.

    //Podriamos llamar este método desde el constructor de la clase, pero lo debemos llamar desde la clase Level
    //porque esta última primero debe colocar la fire ball en la escena.

    QGraphicsItem *item;
    collisions = collidingItems(Qt::IntersectsItemShape);
    for (short i = 0; i < collisions.size(); i++) {

        //Revisamos el tipo de objeto con el que ha colisionado la fire ball.

        //Debemos revisar de esta forma a los enemigos, porque a pesar de que todos tiene como clase base a
        //Enemy, la función typeid devuelve la clase heredada, por lo cual si dejamos solo Enemy, todas las
        //clases de los enemigos especiales no cumplirían la condición.

        item = collisions[i];
        if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail)) or (typeid(*item) == typeid(Porcupine))
         or (typeid(*item) == typeid(Owl)) or (typeid(*item) == typeid(Chamaleon)) or (typeid(*item) == typeid(Mole))
         or (typeid(*item) == typeid(Vulture))) {

            //Aquí sí, como todos los enemigos especiales heredan la clase Enemy, podemos hacer un dinamic_cast<>,
            //y llamar el método reduces_health() pues este método es de la clase base Enemy.

            Enemy *enemy = dynamic_cast<Enemy*>(item);
            enemy->reduces_health(100);
            return;
        }
        else if (typeid(*item) == typeid(PowerUp)) {

            //En caso de que sea un power up, realizamos el dynamic_cast<> para poder llamar el método
            //emit_give_power() de la clase PowerUp, y procedemos a eliminarlo de la escena.

            PowerUp *power_up = dynamic_cast<PowerUp*>(item);

            power_up->emit_give_power();
            scene()->removeItem(power_up);
            delete power_up;

            return;
        }
    }
}

QRectF FireBall::boundingRect() const {
    return QRectF(0, 0, 35, 35);
}

QPainterPath FireBall::shape() const {

    //Le colocamos la forma de una circunferencia a la fire ball.

    QPainterPath path;
    path.addEllipse(QRect(5, 5, 25, 25));
    return path;
}
