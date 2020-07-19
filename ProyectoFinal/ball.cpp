#include "ball.h"
#include <QDebug>

QRectF Ball::boundingRect() const {
    return QRectF(-15, -15, 30, 30);
}

QPainterPath Ball::shape() const {
    QPainterPath path;
    path.addEllipse(QRect(-15, -15, 30, 30));
    return path;
}

Ball::Ball(short x, short y) {

    setPixmap(QPixmap(":/minigame/resources/images/minigame/ball.png"));
    setOffset(-15, -15);
    setPos(x, y);

    restitution = 1;

    //Suponemos una altura de 7 metros para los 497 pixeles que tiene para caer la pelota, luego
    //el factor de conversión es 71 pixeles/metro, por lo cual la gravedad escalada seria 9.8*71 = 695.8,
    //con lo cual, la pelota tarda en caer aproximadamente 1.195229 segundos.

    scaled_gravity = 695.8;

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Ball::move);
}

Ball::~Ball() {
    delete move_timer;
}

void Ball::start_falling(double spd) {

    initial_speed[0] = spd;
    initial_speed[1] = 0;

    initial_pos[0] = x();
    initial_pos[1] = y();

    time = 0;
    move_timer->start(50);
}

void Ball::collision(short side) {

    //Recordemos que la restitución solo afecta en la dirección del choque,
    //por lo cual si la dirección es horizontal la restitución sólo afecta
    //la rapidez en x.

    if (side == 3) {
        initial_pos[0] = 584;
        initial_pos[1] = y();

        initial_speed[0] = -initial_speed[0]*restitution;
        initial_speed[1] = initial_speed[1] + time*scaled_gravity;;

        setX(584);
        time = 0;
    }
    else if (side == 1) {
        initial_pos[0] = 196;
        initial_pos[1] = y();

        initial_speed[0] = -initial_speed[0]*restitution;
        initial_speed[1] = initial_speed[1] + time*scaled_gravity;;

        setX(196);
        time = 0;
    }
    else if (side == 2) {

        //Notemos que como no necesitamos modificar ni la dirección ni magnitud de la componente
        //en X de la velocidad, no aparece.

        //Para que la energía se conserve en caso de que la restitución sea 1, y por tanto tengamos
        //seguridad de que la simulación es correcta para el resto de valores del coeficiente de
        //restitución, utilizamos la relación entre la rapidez en Y y la posición en Y.

        //Este procedimiento no hace falta dentro de los condicionales anteriores porque modificar
        //la posición en X no afecta la energía potencial gravitacional, mientras que la posición en Y sí.

        //No está de más recordar que la fórmula toma esta forma porque tomamos la gravedad positiva en
        //dirección del eje Y positivo.

        initial_speed[1] = -sqrt(2*scaled_gravity*(553 - initial_pos[1]) + initial_speed[1]*initial_speed[1])*restitution;

        initial_pos[0] = x();
        initial_pos[1] = 553;

        setY(553);
        time = 0;
    }
}

void Ball::move() {
    time += 0.06;
    if (time > 1000) return;

    //Recordemos que como el eje Y positivo crece hacia abajo de la pantalla, el término
    //que tiene como factor a la gravedad se debe sumar en lugar de restar.

    setX(initial_pos[0] + time*initial_speed[0]);
    setY(initial_pos[1] + time*initial_speed[1] + 0.5*time*time*scaled_gravity);

    specific_energy = 0.5*((initial_speed[1] + time*scaled_gravity)*(initial_speed[1] + time*scaled_gravity) + initial_speed[0]*initial_speed[0]) - scaled_gravity*y();

    if (584 < x()) collision(3);
    else if (x() < 196) collision(1);
    else if (553 < y()) collision(2);//Justo antes del 569, donde empieza la pared.

    qDebug() << specific_energy;
}






































