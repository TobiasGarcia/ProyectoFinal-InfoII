#include "base.h"

Base::Base(QGraphicsRectItem *_health_bar, short *_health, bool *_extra_life,
           QGraphicsPixmapItem *_lifebuoy, bool _tutorial_level) :

           health_bar(_health_bar), lifebuoy(_lifebuoy), extra_life(_extra_life),
           tutorial_level(_tutorial_level), health(_health) {

    //El punter _health_bar apunta al objeto QGraphicsRectItem del HUD del nivel, que indica la salud de la base;
    //el puntero _health apunta a la salud de la base; _extra_life apunta a un bool que indica si el jugador
    //posee una vida extra o no; _lifebuoy apunta al QGraphicsPixmpa del nivel que representa al savavidas
    //que se coloca sobre la base; _tutorial_level indica si estamos en el tutorial o en un nivel.

    //La variable vulnerable indica si la base puede ser golpeada por los enemigos o no, resulta útil
    //para que cuando se otroga el poder de congelar a los enemigos, estos no le sigan haciendo daño
    //a al base.

    vulnerable = true;

    pix = new QPixmap(":/base/resources/images/base/base.png");
    setPos(360, 240);
    setPixmap(*pix);
    setZValue(0);

    //Cargamos dos pixmaps para la imágen del centro de la base, una de color azul para cuando la base está
    //en estado normal y otra de color café para cuando está siendo golpeada.

    pix = new QPixmap[2];
    pix[0] = QPixmap(":/base/resources/images/base/normal.png");
    pix[1] = QPixmap(":/base/resources/images/base/bitten.png");

    center = new QGraphicsPixmapItem(pix[0], this);
    center->setPos(19, 19);

    //Construimos el objeto hurt_timer, el cual llamará al slot return_normal(), donde este último se encarga
    //de devolver el pixmap del centro de la base a su color azul, después de un cierto período de tiempo
    //en el que la base no ha recibe más golpes.

    hurt_timer = new QTimer;
    hurt_timer->setSingleShot(true);
    connect(hurt_timer, &QTimer::timeout, this, &Base::return_normal);
}

Base::~Base() {
    delete center;
    delete[] pix;
    delete hurt_timer;
}

void Base::increase_health(short increment) {

    //Este método se encarga de incrementar la salud de la base.

    //Actualizamos el valor al que apunta health, y en caso de que este supere el valor de 1000,
    //lo reducimos hasta 1000.

    (*health) += increment;
    if ((*health) > 1000) (*health) = 1000;

    //Consecuentemente modificamos la longitud de la barra del HUD que representa la salud de la base.

    health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
}

void Base::bitten() {

    //Este método se encarga de reducir la salud de la base.

    //Notemos que si la variable vulnerable es false, no realizamos ninguna acción pues la base
    //se encuentra en estado invulnerable.

    if (vulnerable) {

        //Si la base es vulnerable, primero reducimos su salud en 10 puntos.

        (*health) -= 10;

        //En caso de que la base llegue a tener menos de 300 puntos de salud y el jugador cuente con una
        //vida extra, le restauramos toda la salud a la base y le retiramos la vida extra al jugador.

        if (((*health) < 300) and (*extra_life)) {

            //Obsérvese que si nos encontramos en el tutorial, no retiramos la vida extra al jugador,
            //esto es para evitar que la base sea destruida durante el tutorial. En caso de un nivel
            //sí retiramos el salvavidas de la base y la vida extra.

            if (!tutorial_level) {
                scene()->removeItem(lifebuoy);
                (*extra_life) = false;
            }

            //Llamamos el método start() del objeto hurt_timer para que este llame al slot return_normal()
            //que se encarga de devolver la imágen del centro de la base a su color azul.

            hurt_timer->start(500);

            //Restaruamos toda la salud a la base.

            (*health) = 1000;
        }
        else if ((*health) == 0) {

            //Por otro lado, si la salud de la base ha llegado a cero, detenemos el timer hurt_timer para
            //que este no coloque la imágen del centro de la base en azul de nuevo, en caso de que este
            //se encuentre activado, colocamos la base invulnerable para que no llegue a tener una
            //salud negativa, y por último emitimos la señal no_health que le indica a la clase
            //Level que la base ha sido destruida.

            if (hurt_timer->isActive()) hurt_timer->stop();
            vulnerable = false;
            emit no_health();
        }

        //O en el último caso posible, que es cuando la base ha recibid un simple golpe, llamamos el método
        //start() del objeto hurt_timer para que este devuelva la imágen del centro de la base a su color
        //azul de nuevo en pasados 0.5 segundos. Notemos que si la base recibe varios golpes sucesivos
        //este timer se comenzará a reiniciar, mantieniendo el centro de la base en color café, para
        //indicarle al usuario que ésta está recibiendo daño.

        else hurt_timer->start(500);

        //En cualquier caso, como la base fue golpeada, actualizamos la longitud del QGraphicsRectItem
        //del HUD que le indica al usuario la salud de la base, con el nuevo valor al que apunta
        //health, además de colocar el centro de la base de color café pues esta ha recibido
        //daño.

        health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
        center->setPixmap(pix[1]);
    }
}

void Base::vulture_hit() {

    //Este slot es acivado por la señal vulture_hit emitida por la clase heredada de Enemy, Vulture,
    //y su propósito es procesar el golpe de este enemigo, pues él destruye la base de un
    //solo golpe.

    if ((*extra_life)) {

        //En caso de que el jugador cuente con una vida extra, retiramos el salvavidas de la pantalla,
        //le retiramos la vida extra al jugador, llamamos el método start() de hurt_timer y
        //restauramos la salud de la base.

        scene()->removeItem(lifebuoy);
        hurt_timer->start(500);
        (*extra_life) = false;
        (*health) = 1000;
    }
    else {

        //En caso de que el jugador no cuente con una vida extra, colocamos la salud de la base en 0,
        //detenemos el timer hurt_timer en caso de que se encuentre activo y emitimos la señal
        //no_health para indicarle a la clase Level que la base ha sido destruida.

        (*health) = 0;

        if (hurt_timer->isActive()) hurt_timer->stop();
        vulnerable = false;
        emit no_health();
    }

    //Actualizamos la longitud del QGraphicsRectItem del HUD que le indica al usuario la salud de la base,
    //con el nuevo valor al que apunta health, además de colocar el centro de la base de color café.

    health_bar->setRect(221, 544, 337*((*health)/1000.0), 51);
    center->setPixmap(pix[1]);
}

void Base::return_normal() {

    //Este slot es activado por el timer hurt_timer, y su propósito es devolver la imágen del centro de la
    //base a su color azul, pues esta ha pasado un tiempo sin recibir golpes.

    center->setPixmap(pix[0]);
}
