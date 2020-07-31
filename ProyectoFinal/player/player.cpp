#include "player.h"

Player::Player(short i, short j, bool level, bool _player_one) : player_one(_player_one) {

    //Las variables i y j indican la casilla del mapa donde se deben colocar inicialmente a los
    //jugadores; la variable level es true si estamos en un nivel o el tutorial, o es false en
    //caso de que nos encontremos en el menú de niveles, esta diferenciación la debemos
    //realizar pues en un nivel o en el tutorial, en la parte baja de la pantalla se
    //encuentra el HUD, por lo cual el límite inferior para el movimiento de los
    //jugadores es distinto; la variable _player_one indica si la instanciación
    //del objeto player corresponde al primer jugador (true) o
    //al segundo (false).

    if (player_one) pix = new QPixmap(":/players/resources/images/players/blue_scope.png");
    else pix = new QPixmap(":/players/resources/images/players/purple_scope.png");

    setPos(30 + 60*j, 30 + 60*i);
    setOffset(-15, -15);
    setPixmap(*pix);
    setZValue(6);

    //Como se mencionó, si estamos dentro de un nivel o el tutorial, el límite inferior para
    //el movimiento de los jugadores es menor.

    if (level) lower_limit = 525;
    else lower_limit = 585;

    //Inicializamos el arreglo move_dir con todos sus elementos en false para que el objeto
    //Player no se mueva en ninguna dirección hasta que se presione alguna de las teclas
    //para el movimiento.

    move_dir.fill(false);

    //Colocamos freez en false para permitir el movimiento de los jugdores.

    freez = false;

    //Construimos el objeto move_timer, lo conectamos al slot move() que se encargará de mover al
    //objeto Player dependiendo de las teclas presionadas por el usuairo, y llamamos el método
    //start() de este.

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Player::move);
    move_timer->start(17);
}

Player::~Player() {
    delete pix;
    delete move_timer;
}

void Player::move() {

    //Este método se encarga de mover al objeto Player dependiendo de las teclas presionadas por
    //el usuario.

    //Como se mencionó, si la variable freez se encuentra en true, no debemos mover al objeto
    //Player, por lo cual retornamos del método.

    if (freez) return;

    //Aquí procesamos las teclas que actualmente el arreglo move_dir reporta como presionadas,
    //en caso de que se esté presionando en una dirección y no se encuentre en el límite,
    //modificamos la posición del objeto Player.

    if (move_dir[0] and (y() > 15)) setY(y() - 5);
    if (move_dir[1] and (x() > 15)) setX(x() - 5);
    if (move_dir[2] and (y() < lower_limit)) setY(y() + 5);
    if (move_dir[3] and (x() < 765)) setX(x() + 5);
}

QRectF Player::boundingRect() const {
    return QRectF(-18, -18, 36, 36);
}
