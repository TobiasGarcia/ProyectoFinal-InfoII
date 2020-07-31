#include "ball.h"

Ball::Ball(short x, short y, short goal_x, short goal_y) {

    //El valor de x e y indica la posición inicial de la pelota en la escena; los valores de goal_x y goal_y
    //indican la posición del objetivo al que la pelota debe golpear para que el jugador supere el minijuego.

    setPixmap(QPixmap(":/minigame/resources/images/minigame/ball.png"));
    setOffset(-15, -15);
    setPos(x, y);

    //La variable rebounds_num nos indicará la cantidad de rebotes de la pelota, la inicializamos
    //con el valor de 0.

    rebounds_num = 0;

    //Almacenamos la posición del objetivo en el arreglo goal_pos.

    goal_pos[0] = goal_x;
    goal_pos[1] = goal_y;

    //Tomamos un coeficiente de restitución de 0.8 para los rebotes de la pelota contra las paredes.

    restitution = 0.8;

    //Suponemos una altura de 7 metros para los 497 pixeles que tiene para caer la pelota, de donde el factor de
    //conversión resulta ser 71 pixels/metro, por lo cual la gravedad escalada seria 9.8*71 = 695.8 pixels/(s^2),
    //y por tanto, la pelota tardaría en caer aproximadamente 1.195229 segundos si no rebota contra ninguna
    //pared.

    scaled_gravity = 695.8;

    //Construimos el objeto move_timer para que llame al slot move(), el cual se encargará de modificar la
    //posición de la pelota pra simular su movimiento.

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Ball::move);
}

Ball::~Ball() {
    delete move_timer;
}

void Ball::start_falling(double spd) {

    //Este método recibe una variable de tipo double que indica la componente horizontal de la velocidad
    //inicial con que se deja caer a la pelota.

    //La velocidad inicial será almacenada en el arreglo initial_speed. La componente vértical es 0 pues
    //la pelota se deja caer.

    initial_speed[0] = spd;
    initial_speed[1] = 0;

    //Tomamos la posición inicial y la almacenamos en el arreglo initial_pos. Notemos que esta posición
    //no tiene porque ser la definida en el constructor, pues como se indica en la documentación de la
    //clase Minigame, la pelota se estará moviendo solidariamente con una garra que la sostendrá hasta
    //que el usuario presione una tecla especificada.

    initial_pos[0] = x();
    initial_pos[1] = y();

    //Colocamos la variable time en cero y llamamos el método start() del objeto move_timer para comenzar
    //con el movimiento de la pelota.

    time = 0;
    move_timer->start(50);
}

void Ball::move() {

    //Este slot se encarga de actualizar la posición de la pelota para simular su movimiento.

    //Este slot es llamado cada 0.05 segundos, por lo cual el valor de la variable time debería aumentar en
    //principio 0.05 cada vez, sin embargo, para darle una ventana de tiempo a los procesos que tenga que
    //hacer la computadora, utilizamos 0.06 en su lugar.

    time += 0.06;

    //Utilizamos las ecuaciones cinématicas que describen por completo el movimiento parabólico.

    //Recordemos que como el eje Y crece hacia abajo de la pantalla, el término que tiene como factor a la
    //componeten vértical de la gravedad se debe sumar, pues esta componente es positivo según el sistema
    //de referencia.

    setX(initial_pos[0] + time*initial_speed[0]);
    setY(initial_pos[1] + time*initial_speed[1] + 0.5*time*time*scaled_gravity);

    //En caso de que se cumpla alguna de las siguientes condiciones, es porque la pelota ha hecho contacto
    //con una pared, por lo cual llamamos el método collision() para hacer las modificaciones necesarias
    //para simular el rebote de la pelota.

    if (584 < x()) collision(3);
    if (x() < 196) collision(1);
    if (553 < y()) collision(2);

    //Verificamos si la pelota ha hecho contacto con el objetivo, revisando si la norma de de la resta de ambos
    //vectores de posición es inferior a la suma de los radios de la pelota y el objetivo (15 + 10 = 25).

    //Utilizamos este condicional en lugar del método collidingItems(), pues no pienso que sea
    //necesario recurrir a este método sabiendo que esta condición es bastante sencilla.

    if (sqrt(pow(goal_pos[0] - x(), 2) + pow(goal_pos[1] - y(), 2)) < 25) {

        //En caso de que la pelota haya hecho contacto con el objetivo detenemos el movimiento de
        //la pelota llamando al método stop() de move_timer, y emitimos la señal win para
        //notificar a la clase Minigame de que el minijuego ha sido superado.

        move_timer->stop();
        emit win();
    }
}

void Ball::collision(short side) {

    //Este método es llamado cuando la pelota se ha chocado contra una pared; el valor de la variable
    //side indica la dirección del choque, 1 para un choque por la izquierda, 2 para un choque por
    //debajo y 3 para un choque por la derecha.

    //Como la pelota se ha chocado, aumentamos el valor de la variable rebounds_num.

    rebounds_num++;

    //Recordemos que el coeficiente de restitución sólo afecta la componente de la velocidad de la dirección
    //del choque, por lo cual si la dirección es horizontal el coeficiente de restitución sólo afecta la
    //componente horizontal de la velocidad; análogamente para los choques vérticales.

    //Si side es impar es porque es un choque horizontal.

    if (side%2) {

        //Colocamos la pelota justo al borde de la pared, porque puede suceder que la imágen se haya
        //sobrepuesto un poco sonbre esta debido a una gran rapidez.

        initial_pos[0] = 194*side + 2;
        initial_pos[1] = y();

        //Tomamos como nueva velocidad inicial la velocidad actual de la pelota, pero disminuimos la componente
        //horizontal multipicándola por el coeficiente de restitución, además de cambiarle el signo para que la
        //pelota rebote.

        initial_speed[0] = -initial_speed[0]*restitution;
        initial_speed[1] = initial_speed[1] + time*scaled_gravity;

        setX(initial_pos[0]);

        //Reiniciamos la variable time para comenzar con el nuevo movimiento.

        time = 0;
    }
    else {

        //En este caso, cuando el choque es por debajo, como no hay necesidad de modificar ni la dirección
        //ni la magnitud de la componente en X de la velocidad, esta no aparece.

        //Para que la energía se conserve en caso de que la restitución sea 1, y por tanto tengamos
        //seguridad de que la simulación es correcta para el resto de valores del coeficiente de
        //restitución, utilizamos la relación entre la rapidez en Y y la posición en Y, para
        //calcular la componente en Y de la velocidad cuando ajustemos la altura para
        //colocar a la pelota justo en el borde del suelo, por la misma razón que se
        //ajusta la posición en X en los choques horizontales.

        initial_speed[1] = -sqrt(2*scaled_gravity*(553 - initial_pos[1]) + initial_speed[1]*initial_speed[1])*restitution;

        //Este procedimiento no hace falta en los choques horizontales puesto que modificar la posición en X
        //no afecta la energía potencial gravitacional, mientras que la posición en Y sí.

        //No está de más recordar que la fórmula anterior toma esa forma porque la componente vértical de la
        //aceleración de la gravedad es positiva según el sistema de referencia.

        initial_pos[0] = x();
        initial_pos[1] = 553;

        setY(553);

        //Reiniciamos la variable time para comenzar con el nuevo movimiento.

        time = 0;
    }
}

void Ball::stop_timer() {

    //Este método es llamado desde la clase Minigame cuando el jugador ha superado o perdido
    //en una de las oportunidades del minijuego, su propósito es detener el movmiento de la
    //pelota llamando al método stop() del objeto move_timer.

    move_timer->stop();
}


QRectF Ball::boundingRect() const {
    return QRectF(-15, -15, 30, 30);
}

QPainterPath Ball::shape() const {

    //Colocamos la forma de la pelota como una circunferencia.

    QPainterPath path;
    path.addEllipse(QRect(-15, -15, 30, 30));
    return path;
}
