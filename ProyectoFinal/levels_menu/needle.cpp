#include "needle.h"

Needle::Needle() {

    pix = new QPixmap(":/levels_menu/resources/images/levels_menu/needle.png");
    setPos(390, 300);
    setPixmap(*pix);

    //Como se menciona en needle.h, comenzaremos con un MAS. Tomaremos un períodod de 4 segundos, obteniendo
    //así una frecuencia angular de 2*pi/4 = 1.570796 rad/s aproximadamente. Para la amplitud tomaremos un
    //valor de 75 grados. Seleccioné estos valores de esta forma porque me parecía que así se verá bien el
    //movimiento desde la perspectiva del usuario.

    amp = 75;
    period = 4;
    ang_frecuency = 2*M_PI/period;

    //Como aún no estamos en el movimiento subamortiguado, tomamos un valor de 0 para gamma, pues este parámetro
    //representará la presencia de fricción como se explica en la documentación del método point_direction(),
    //además de un valor paara phase también en 0 pues esta variable la utilizaremos para realizar la
    //transición entre un MAS y un movimiento subamortiguado dentro del método point_direction().

    //Colocamos offset en 0 pues queremos que el punto de equilibrio se encuentre con la aguja apuntando
    //hacia abajo.

    time = 0;
    gamma = 0;
    phase = 0;

    offset = 0;

    //Construimos el timer move_timer pues este llamará al slot move(), el cual se encargará de modificar la
    //posició angular de la aguja para simular su movimiento.

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Needle::move);
    move_timer->start(50);
}

Needle::~Needle() {
    delete pix;
    delete move_timer;
}

void Needle::move() {

    //Este slot se encargará de actualizar la posición angular de la aguja para simular su movimiento.

    //Este slot es llamado cada 0.05 segundos, por lo cual el valor de la variable time debería aumentar en
    //principio 0.05 cada vez, sin embargo, para darle una ventana de tiempo a los procesos que tenga que
    //hacer la computadora, utilizamos 0.06 en su lugar.

    time += 0.06;

    //En caso de que la variable gamma aún sea 0, significa que aún nos encontramos en el MAS, por lo cual
    //si el valor de la variable time supera el período de oscilación, le restamos este último para evitar
    //que su valor aumente indefinidamente.

    if ((gamma == 0) and (time > period)) time -= period;

    //Actualizamos la posición angular de la aguja según el modelo que describe el MAS. El 180 es porque
    //queremos que en principio la posición de equilibrio (cero grados) sea apuntando hacia a abajo, y
    //el menos es para que la oscilación comienza en sentido horario.

    //NOTA: El método setRotation() realiza una rotación al objeto en sentido horario midiendo el angulo
    //ingresado en grados respecto al semieje Y negativo del sistema coordenado de la imágen.

    //La variable offset adquiere un valor distinto de 0 dentro el método point_direction(), y nos
    //permite modificar el punto de equilibrio para que la aguja oscile hacia otra dirección.

    setRotation(180 - (amp*exp(-gamma*time)*cos(ang_frecuency*time + phase) + offset));

    //El siguiente condicional es para que cuando ya se pasó al movimiento subamortiguado (gamma distinto
    //de cero), coloquemos un límite inferior a la amplitud para detener el movimiento completamente,
    //y enviar la señal finish al objeto de la clase LevelsMenu para indicarle que ya la aguja ha
    //terminado con su animación.

    if ((gamma != 0) and (amp*exp(-gamma*time) < 0.8)) {
        move_timer->stop();
        emit finish();
    }
}

void Needle::point_direction(short direction) {

    //Este método se encarga de modificar los valores de algunas variables para realizar la transición del
    //MAS a una oscilación subamortiguada, además de darle un valor adecuado a la variable offset para que
    //la aguja comience a oscilar en una dirección en particular.

    //Primeramente almacenamos la posición angular incial en la variable initial_angle y la rapidez que
    //lleva la aguja en este momento en la variable initial_speed. Notemos que no debemos sumar el
    //valor de la variable phase ni utilizar la exponendial de la amplitud, pues cuando se ingresa
    //a este método aportan un valor de 0 y 1 respectivamente.

    double initial_angle = amp*cos(ang_frecuency*time),
            initial_speed = -ang_frecuency*amp*sin(ang_frecuency*time);

    //La variable direction puede tomar los siguiente cuatro valores que indican una dirección: 0 para arriba,
    //1 para izquierda, 2 para abajo y 3 para derecha, de esta forma podemos ajustar el valor de la variable
    //offset para que el punto de equilibrio de la aguja se traslade a la dirección correspondiente.

    //NOTA: Puede resultar un poco difícil ver por qué los siguientes valores de offset funcionan, pero con
    //análisis detalladao y siendo consientes de que el MAS lo medimos como si los 0 grados fueran apuntnado
    //hacia abajo, los angulos positivos son en sentido antihorario y los negativos en horario, mientras que
    //el método setRotation() interpreta los 0 grados cuando la aguja apunta hacia arriba y los angulos
    //positivos son en horario mientras que los negativos en antihorario, se puede ver por qué estos
    //valores (en grados) fueron seleccionados.

    if (direction == 3) offset = 90;
    else if (direction == 1) offset = -90;

    //Las siguietnes dos condiciones son para que la sentencia initial_angle -= offset
    //funcione para todos los valores de offset.

    else if ((direction == 0) and (initial_angle >= 0)) offset = 180;
    else if ((direction == 0) and (initial_angle < 0)) offset = -180;

    initial_angle -= offset;

    //Le damos un valor de 2 s^-1 a la variable gamma para comenzar a considerar la fricción.

    gamma = 2;

    //Para el movimiento subamortiguado utilizaremos un período de 0.8 segundos, por lo cual la nueva
    //'frecuencia angular' será de 2*pi/0.8 = 7.853982 rad/s aproximadamente.

    period = 0.8;
    ang_frecuency = 2*M_PI/period;

    //Calculamos la nueva amplitud del movimiento según los valores de las variables initial_angle,
    //initial_speed, gamma y ang_frecuency.

    amp = sqrt(initial_angle*initial_angle + pow(initial_speed + gamma*initial_angle, 2)/(ang_frecuency*ang_frecuency));

    //Colocamos el valor de phase adecuado según el valor de initial_speed + gamma*initial_angle
    //sea positivo o no.

    if ((initial_speed + gamma*initial_angle) > 0) phase = -acos(initial_angle/amp);
    else phase = acos(initial_angle/amp);

    //Reiniciamos el valor de la variable time para comenzar el nuevo movimiento subamortiguado.

    time = 0;
}

QRectF Needle::boundingRect() const {
    return QRectF(-22, -127, 44, 254);
}

void Needle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    //La siguiente sentencia es con el propósito de que se mantenga una buena resolución al realizar
    //las rotaciones de la imágen de la aguja.

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(-22, -127, *pix);
}
