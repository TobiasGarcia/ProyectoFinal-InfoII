#include "powerup.h"

PowerUp::PowerUp(short _path_type, short _power_type) : path_type(_path_type), power_type(_power_type) {

    pix = new QPixmap(":/power_ups/resources/images/power_ups/power" + QString::number(power_type) + ".png");

    setOffset(-11, -11);
    setPixmap(*pix);
    setZValue(3);

    //Recordemos que las capas para colocar los Z values son presentadas en la documentación del constructor
    //de la clase Enemy.

    //Definimos los parámetros a y b de la trayectoria según su tipo, el tipo viene determinado por el valor
    //de la variable path_type, la cual puede tomar los siguientes valores: 0 para la elipse, 1 par el
    //astroide, 2 para la rosa polar de 5 pétalos y 3 para la lemniscata de Bernoulli.

    define_parameters();

    //Utilizamos move_timer para llamar al slot move() cada 50 milisegundos, cuyo propósito es actualizar la
    //posición de la imágen. Como se mencionó en power_up.h, la lemniscata de Bernoulli fue la trayectoria
    //más complicada porque no era una función, por lo cual no era compatible con la forma estándar del
    //slot move(), por lo cual utilizamos el slot lemniscate_move para su caso particular.

    move_timer = new QTimer;

    if (path_type != 3) connect(move_timer, &QTimer::timeout, this, &PowerUp::move);
    else connect(move_timer, &QTimer::timeout, this, &PowerUp::lemniscate_move);

    //El display_timer lo utilizamos para que el power up se elimine de la escena pasado cierto tiempo,
    //esto es para que el jugador tenga un límite de tiempo para adquirirlo.

    display_timer = new QTimer;
    display_timer->setSingleShot(true);

    //Comenzamos el movimiento llamando el método start() del timer move_timer.

    freez = false;
    move_timer->start(50);
}

PowerUp::~PowerUp() {
    delete pix;
    delete move_timer;
    delete display_timer;
}

void PowerUp::define_parameters() {

    //Este método se encarga de determinar los parámetros a y b, además de darle un valor inicial aleatorio
    //pero válido a la variable angle, para comenzar a describir la trayectoria especificada por el valor
    //de path_type.

    if (path_type == 0) {

        //Parámetros para la elipse; ángulo inicial aleatorio entre 0 y 2*pi; período 2*pi.

        a = 129; b = 69; spd = 300;
        angle = (rand()%360)*M_PI/180;
        period = 2*M_PI;
        r = radio();
    }
    else if (path_type == 1) {

        //Parámteros para el astroide; ángulo inicial aleatorio en alguno de los intervalos (en grados):
        //[30, 60], [120, 150], [210, 240] o [300, 330]; periodo 2*pi. Estos intervalos son porque la
        //derivada del astroide posee singularidades en los ángulos (en grados): 0, 90, 180 y 270,
        //aunque si no se comienza precisamente en alguno de esos ángulos no se presentan
        //problemas pues los valores arrojados por las funciones sin() y cos(), según
        //nuestro intervalo de tiempo, no son lo suficientemente cercanos a 0 para
        //lanzar una excepción. El período de esta curva es 2*pi.

        a = 260; spd = 300;
        angle = ((rand()%31 + 30) + 90*(rand()%4))*M_PI/180;
        period = 2*M_PI;
        r = radio();
    }
    else if (path_type == 2) {

        //Parámetros para la rosa polar de 5 pétalos; ángulo inicial aleatorio entre 0 y 3*pi; período 3*pi.

        a = 250; b = 5.0/3.0; spd = 300;
        angle = (rand()%540)*M_PI/180;
        period = 3*M_PI;
        r = radio();
    }
    else if (path_type == 3) {

        //Parámetros para la lemniscata de Bernoulli; ángulo inicial aleatorio entre -pi/4 y pi/4.
        //Como se puede ver en la documentación del slot lemniscate_move(), el ángulo aumenta
        //desde -pi/4 hasta pi/4, y luego vuelve a disminuir hasta -pi/4, por lo cual no
        //requerimos de la variable period para que el ángulo no aumente
        //indefinidamente, puesto que ete siempre oscilará entre -pi/4 y
        //pi/4, por lo cual la inicializamos con un valor cualesquiera.

        //Notemos que le damos un valor aleatorio entre el -1 y el 1 al parámetro b, la razón de esto
        //es explicada dentro de la documentación del slot lemniscate_move().

        a = 300; b = 2*(rand()%2) - 1; spd = 300;

        angle = (rand()%61 - 30)*M_PI/180;
        r = a*b*sqrt(cos(2*angle));
        period = 0;
    }

    setPos(389 + r*cos(angle), 269 - r*sin(angle));
}

double PowerUp::radio() {

    //Este método retorna el valor de la función en coordenadas polares de la trayectoria especificada
    //por el valor de la variable path_type, evaluada en el ángulo angle. Notése que no se encuentra
    //la lemniscata de Bernoulli pues como se ha comentado, esta trayectoria recibe un tratamiento
    //distinto a las demás.

    if (path_type == 0) return 1/sqrt(pow(cos(angle)/a, 2) + pow(sin(angle)/b, 2));
    else if (path_type == 1) return a/pow(pow(cbrt(cos(angle)), 2) + pow(cbrt(sin(angle)), 2), 1.5);
    else return a*cos(b*angle);
}

double PowerUp::diff_radio() {

    //Este método retorna la derivada de la función en coordenadas polares, de la respectiva trayectoria,
    //respecto al ángulo evaluada en la variable angle. Notése que no se encuentra la lemniscata de
    //Bernoulli pues como se ha comentado, esta trayectoria recibe un tratamiento distinto a las
    //demás.

    if (path_type == 0) return (0.5*a*b*(b*b - a*a))*(sin(2*angle)/pow(pow(b*cos(angle), 2) + pow(a*sin(angle), 2) , 1.5));
    else if (path_type == 1) {
        c = cos(angle); s = sin(angle);
        return a*((s/cbrt(c)) - (c/cbrt(s)))/pow(pow(cbrt(c), 2) + pow(cbrt(s), 2), 2.5);
    }

    //En el caso de la rosa polar de 5 pétalos, la derivada lleva un signo menos, pero como
    //nos interesa elevarla al cuadrado resulta indiferente el colocarlo o no.

    else return a*b*sin(b*angle);
}

void PowerUp::move() {

    //Este slot actualiza la posición del objeto dentro de la escena para que describa la trayectoria
    //especificada por el valor de la variable path_type.

    //Si freez es true, no debemos mover el objeto, por lo cual simplemente retornamos.

    if (freez) return;

    //En el método define_parameters() ya hemos calculado el valor de r, por lo cual solo queda calcular
    //el valor de dr y utilizar la fórmula presentada al principio de power_up.h.

    //NOTA: Como el slot move() es llamado cada 0.05 segundos, el valor de Dt debe ser en principio 0.05,
    //pero para darle una ventana de tiempo a los procesos que tenga que hacer la computadora,
    //utilizamos 0.06 en su lugar.

    dr = diff_radio();
    angle += 0.06*spd/sqrt(dr*dr + r*r);

    //Como mencionamos en power_up.h, para que el ángulo no aumente de forma indefinida, si este
    //supera el período de la curva, se lo restamos para reducir su valor.

    if (angle > period) angle -= period;

    //Luego de haber calculado el nuevo ángulo, calculamos el valor de r de nuevo y modificamos la
    //posición del objeto.

    r = radio();
    setPos(389 + r*cos(angle), 269 - r*sin(angle));
}

void PowerUp::lemniscate_move() {

    //Este slot sustituye al slot move() para el caso de la lemniscata de Bernoulli, esto es debido a que como
    //su ecuación en coordenadas polares no es una función, debemos darle un trato diferente para obtener la
    //curva completa.

    //De forma análoga a move(), si freez es true debemos retornar.

    if (freez) return;

    //La ecuación de la lemniscata de Bernoulli en coordenadas polares es (r(q))^2 = (a^2)*cos(2*q), por lo
    //cual debemos dividir la trayectoria en dos: r(q) = a*sqrt(cos(2*q)) y r(q) = -a*sqrt(cos(2*q)), para
    //esto utilizamos el parámetros b, el cual puede tomar el valor de -1 o 1, de esta forma siempre
    //podremos utilizar r(q) = b*a*sqrt(cos(2*q)) y variar el valor de b para conseguir ambas
    //fragmentos de la curva, por esto es que dentro del método define_parameters() tomamos
    //un valor aleatorio para b entre el -1 y el 1, para empezar en cualquiera de los dos
    //fragmentos.

    //A diferencia del astroide, en este caso la función cos() sí llega a arrojar valores lo suficientemente
    //pequeños como para lanzar una indeterminación durante el cálculo del valor de dr, esto es debido a que
    //cerca de los valores de indeterminación de dr, la variación del ángulo resulta ser muy pequeña (menor
    //a 0.01 radianes), algo que no sucede en el caso del astroide (su valor mínimo oscila alrederdor de
    //0.01 radianes cerca de sus puntos de indeterminación).

    //Para dar frente al problema anterior, lo que hacemos es que cerca de los valores de indeterminación
    //(-pi/4 y pi/4), cambiamos a una trayectoria rectilínea, cruzamos el punto de indeterminación y volvemos
    //a encajar con la curva de la lemniscata de Bernoulli, luego de encontrarnos a una distancia prudente de
    //la singularidad. Como se puede ver durante la ejecución del programa, el cambio de trayectoria es tan
    //sutil, y como la forma de la lemniscata de Bernoulli muy cerca del centro es casi indentica a una línea
    //recta (no necesariamente de pendiente 1, puede ser más o menos inclinada), desde la perspectiva del
    //usuario el cambio de trayectoria es casi imperceptible.

    //Por tanteo determinamos que una distancia prudente para el cambio de trayectoria es a 40 pixeles del
    //origen de nuestro sistema de coordenadas (recordar que el origen del sistema de coordenadas de los cálculos
    //de esta clase es el pixel x = 389 e y = 269 de la escena, como se mecniona en power_up.h), por lo cual
    //cuando entramos en esta región realizamos el cambio de trayectoria.

    if (abs(r) < 40) {

        //La variable straight_line nos indicará cuando estamos en la trayectoria rectilínea o no, true para
        //trayecotria rectilínea y false para la lemniscata de Bernoulli.

        if (!straight_line) {

            //Si estamos a 40 pixeles del origen y straight_line es false, realizamos el cambio de trayecotria.
            //Alternamos el valor de b pues luego de la trayecotria rectilínea comenzaremos con el otro
            //fragmento de la lemniscata de Bernoulli, y de paso calculamos el ángulo inicial de este
            //fragmento.

            b *= -1;
            angle +=  b*0.06*spd/sqrt(dr*dr + r*r);

            //Calculamos el ángulo de la línea recta que seguiremos y lo almacenamos en m_angle.

            m_angle = atan2(269 - y(), x() - 389);
        }

        //Colocamos la variable straight_line para que el procedimiento anterior sólo se ejecute
        //la primera vez, y comenzamos a seguir la trayectoria rectilínea.

        straight_line = true;
        setPos(x() - 0.06*spd*cos(m_angle), y() + 0.06*spd*sin(m_angle));
        r = sqrt(pow(x() - 389, 2) + pow(269 - y(), 2));
    }
    else {

        //En caso de que nos encontremos suficientemente alejados de la singularidad, continuamos
        //con la curva de la lemniscata de Bernoulli.

        //De forma similar al caso de la rosa polar de 5 pétalos, la derivada de la lemniscata lleva un signo
        //menos, pero como nos interesa en particular su segunda potencia, es indiferente el colocarlo o no.

        straight_line = false;
        dr = a*sin(2*angle)/sqrt(cos(2*angle));
        angle += b*0.06*spd/sqrt(dr*dr + r*r);

        r = a*b*sqrt(cos(2*angle));
        setPos(389 + cos(angle)*r, 269 - sin(angle)*r);
    }
}

//Este método es para no tener que conectar siempre todas las fireballs al slot
//del nivel, pues las fireballs como tal casi nunca lanzarian la señal pues muchas
//se usan es para los enemigos o para tirarlas a la nada, mientras que el power_up
//si es mucho más probable que lance la señal, pues está diseñado para que lo haga.

void PowerUp::emit_give_power() {

    //Este método emite la señal give_power para activar el slot give_power() de la clase Level.

    //Este método es llamado desde el método test_collisions() de la clase FireBall, por lo cual
    //podríamos haberle colocado la señal give_power directamente a esta, sin embargo, no opté por
    //esa opción pues los objetos de la clase FireBall son construidos y eliminados muchas veces
    //y habría que andar conectándolos cada vez que se construyan, además de que la señal sólo
    //tiene sentido si hay un power up en la escena, por lo cual decidí colocarle la señal a
    //la clase PowerUp y un método público para activarla desde el método test_collisions()
    //de la clase FireBall.

    emit give_power(power_type);
}

QPainterPath PowerUp::shape() const{

    //Mediante esta reimplementación de método shape(), le colocamos la forma
    //de una cricunferencia al power up.

    QPainterPath path;
    path.addEllipse(QRect(-11, -11, 22, 22));
    return path;
}
