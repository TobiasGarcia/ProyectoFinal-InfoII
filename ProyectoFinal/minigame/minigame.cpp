#include "minigame.h"

Minigame::Minigame(short _two_players, short *_rocks_num, short *_glues_num, bool *_extra_life) :
    rocks_num(_rocks_num), glues_num(_glues_num), two_players(_two_players), extra_life(_extra_life) {

    //_two_players indica si el juego se encuentra en modo multijugador o de un solo jugador; _rocks_num apunta
    //a la cantidad de rocas que posee el jugador; análogo para _glues_num pero para las cargas de pegament0;
    //el valor hacia donde apunta _extra_life indica si el jugador posee una vida extra o no.

    //Los punteros son para poder modificar las variables de estado de la partida desde esta misma clase.

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/minigame/resources/images/minigame/minigame_background.png")));

    //La variable state nos ayudará a manejar el flujo del minijuego indicándonos en que estado nos
    //encontramos; los estados posibles son los siguientes:

    //0: Estamos mostrando el mensaje inicial y esperamos unos segundos antes de empezar.
    //1: Estamos en la primera oportunidad para jugar el minijuego, espramos a que se suelte la pelota.
    //2: Ya se soltó la pelota y está rebotando, esperamos a que se gane o se pierda.
    //3: Se muestra la información respecto a haber ganado o perdido; esperamos a que se presione la tecla
    //   especificada en el mensaje.
    //4: Mostramos un mensaje y esperamos unos segundos antes de la segunda oportunidad.
    //5: Estamos en la segunda oportunidad para jugar el minijuego, espramos a que se suelte la pelota.
    //6: Ya se soltó la pelota y está rebotando, esperamos a que se gane o se pierda.
    //7: Se muestra la información respecto a haber ganado o perdido; esperamos a que se presione la tecla
    //   especificada en el mensaje.

    state = 0;

    //La variable claw_close es utilizada para indicar cuando la garra se encuentra cerrada.

    claw_close = true;

    initialize_objects();

    display_initial_message();

    //Como se mencionó en minigame., la garra simulará el movimiento de un sistema masa-resorte, se tomará
    //una masa de 10 kg; para que se vea bien el movimiento se usará un período de 3 segundos, por lo cual
    //la constante el resorte será k = m*4*(pi^2)/(period^2) = 43.864908 N/m aproximadamente. Se utilizará
    //una amplitud inicial de 155 pixels.

    m = 10;
    period = 3;
    k = m*4*M_PI*M_PI/(period*period);

    ang_frecuency = sqrt(k/m);
    amp = 155;

    //Comenzamos con un valor de gamma de 0 para no considerar la fricción hasta que se suelte la pelota,
    //además de una fase inicial de 0 randianes pues esta variable será modificada dentro del método
    //drop_ball() para realizar la transición entre el MAS y la oscilación subamortiguada.

    time = 0;
    gamma = 0;
    phase = 0;

    //Retiramos la pantalla negra mediante el método change_opacity() del objeto black_screen,
    //y comenzamos a esperar 3 segundos, antes de iniciar con el movimiento de la garra,
    //llamado al método start() del objeto delay_timer.

    black_screen->change_opacity(false);
    delay_timer->start(3000);
}

void Minigame::initialize_objects() {

    //Este método construye, y agrega a la escena según sea el caso, todos los objetos que utiliza
    //la clase Minigame.

    information = new Information(this);

    black_screen = new BlackScreen;
    connect(black_screen, &BlackScreen::finish, this, &Minigame::minigame_finished);
    addItem(black_screen);

    //Construimos los timers. El timer claw_move_timer será uitlizado para llamar al slot claw_move(), el cual
    //tiene como propósito modificar la posición de la garra para simular su movimiento. El timer delay_timer
    //es utilizado para hacer unos pequeños delays entre ciertas transiciones de estado del minijuego.

    claw_move_timer = new QTimer;
    connect(claw_move_timer, &QTimer::timeout, this, &Minigame::claw_move);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Minigame::next_state);

    //Construimos el resto de objetos necesarios.

    claw_pix = new QPixmap[2];
    claw_pix[0] = QPixmap(":/minigame/resources/images/minigame/claw0.png");
    claw_pix[1] = QPixmap(":/minigame/resources/images/minigame/claw1.png");

    claw = new QGraphicsPixmapItem;
    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw->setPos(389, 30);
    addItem(claw);

    //Colocamos el objetivo en una Y de 440 pixels y una X aleatoria entre 200 y 580 pixels, inclusive.

    goal = new QGraphicsPixmapItem(QPixmap(":/minigame/resources/images/minigame/goal.png"));
    goal->setPos(rand()%381 + 200, 440);
    goal->setOffset(-10, -10);
    addItem(goal);

    ball = new Ball(389, 103, goal->x(), goal->y());
    connect(ball, &Ball::win, this, &Minigame::win);
    addItem(ball);
}

void Minigame::display_initial_message() {

    //Este método define y agrega el mensaje inicia a la escena, en función
    //al valor de la variable two_players.

    if (two_players) {
        message = "                    MINIJUEGO\n"
                  "             ¡Dale al Objetivo!\n"
                  "Primer Jugador (Tecla Enter)";
    }
    else {
        message = "        MINIJUEGO\n"
                  "¡Dale al Objetivo!";
    }

    //Colocamos el mensaje en la escena mediante el llamado al método display_message()
    //del objeto information.

    information->display_message(389, 240, message);
}

Minigame::~Minigame() {
    delete ball;
    delete goal;
    delete claw;
    delete information;
    delete black_screen;

    delete claw_move_timer;
    delete delay_timer;

    delete[] claw_pix;
}

void Minigame::claw_move() {

    //Este slot se encargará de actualizar la posición de la garra para simular su movimiento.

    //Este slot es llamado cada 0.05 segundos, por lo cual el valor de la variable time debería aumentar en
    //principio 0.05 cada vez, sin embargo, para darle una ventana de tiempo a los procesos que tenga que
    //hacer la computadora, utilizamos 0.06 en su lugar.

    time += 0.06;

    //Si la garra está cerrada, es porque aún estamos en el MAS, por lo cual si el valor de la variable
    //time supera el período de oscilación, le restamos este último para que no aumente indefinidamente

    if ((claw_close) and (time > period)) time -= period;

    //Actualizamos la posición de la garra para que describa un MAS.

    claw->setX(389 + amp*exp(-gamma*time)*sin(ang_frecuency*time + phase));

    //Si la garra se encuentra cerrada, es porque aún no hemos soltado la pelota o se ha terminado
    //la oportunidad para jugar la minijuego; en este último caso este slot no se ejecuta, por lo
    //cual si aquí claw_close llega con el valor de false es porque aún no hemos soltado la
    //pelota, por lo cual debemos actualizar su posición también para que se mueva de forma
    //solidaria con la garra.

    if (claw_close) ball->setX(claw->x());

    //Si por otro lado, la garra ya soltó a la pelota, verificamos si la amplitud de oscilación
    //de la garra es menor a su límite inferior establecido, para llamar el método fail(), pues
    //eso significa que el jugador no ha logrado superar el minijuego en esta oportunidad.

    else if (exp(-gamma*time) < 0.02) fail();
}

void Minigame::keyPressEvent(QKeyEvent *event) {

    //Esta método se encarga de procesar las teclas presionadas por el jugador.

    if (event->isAutoRepeat()) return;

    //Recordemos que la tecla para el Enter cercano a las flechas corresponde a Qt::Key_Return,
    //Qt::Key_Enter es utilizado para el Enter del Numeric Keypad.

    //En caso de que se presione la tecla Enter y estemos en el estado 1, debemos soltar la pelota.

    if ((event->key() == Qt::Key_Return) and (state == 1)) drop_ball();

    //En caso de que se presione la tecla Enter y estemos en el estado 3, debemos comenzar con la
    //segunda oportunidad que brinda el minijuego.

    else if ((event->key() == Qt::Key_Return) and (state == 3)) second_chance();

    //En caso de que se presione la tecla Enter y estemos en el estado 5, debemos soltar la pelota
    //por segunda vez pues nos encontramos en la segunda oportunidad que brinda el minijuego.
    //Recordemos que si estamos en el modo de dos jugadores, la segunda oportunidad la
    //toma el segundo jugador mediante la tecla Espacio.

    else if ((event->key() == Qt::Key_Return) and !two_players and (state == 5)) drop_ball();

    //En caso de que se presione la tecla Enter y estemos en el estado 7, debemos comenzar el
    //proceso para salir del minijuego.

    else if ((event->key() == Qt::Key_Return) and (state == 7)) black_screen->change_opacity(true);

    //Las siguientes condiciones aplican para el caso en que el juego se encuentra en el modo de
    //dos jugadores.

    //En caso de que se presione la tecla Espacio y estemos en el estado 3, si estamos en el modo de
    //dos jugadores, también debemos comenzar con la segunda oportunidad que brinda el minijuego.

    else if (two_players and (event->key() == Qt::Key_Space) and (state == 3)) second_chance();

    //En caso de que se presione la tecla Espacio y estemos en el estado 5, debemos soltar la pelota
    //pero pues el segundo jugador la ha lanzado.

    else if (two_players and (event->key() == Qt::Key_Space) and (state == 5)) drop_ball();

    //Finalmente, si estamos en el estado 7 y se presiona la tecla Espacio, si estamos en el modo de
    //dos jugadores, también comenzamos con el proceso para termianr el minijuego.

    else if (two_players and (event->key() == Qt::Key_Space) and (state == 7)) black_screen->change_opacity(true);
}

void Minigame::drop_ball() {

    //Este método se encarga de soltar la pelota y realizar la transición del MAS de la garra a una
    //oscilación subamortiguada.

    //Primero modificamos la imágen de la garra para colocarla abierta, además de colocar,
    //consecuentemente, la variable claw_close en false.

    claw->setPixmap(claw_pix[1]);
    claw->setOffset(-50, 0);
    claw_close = false;

    //De forma similar al proceso que se siguen en el método point_direction() para la transición del
    //MAS a la oscilación amortigada, almacenamos la posición y rapidez que se lleva en el momento
    //en las variables initial_pos e initial_speed.

    //No hace falta la variable phase ni la exponencial de la amplitud pues cuando este método es llamado
    //sus respectivos valores son 0 y 1.

    double initial_pos = amp*sin(ang_frecuency*time),
           initial_speed = amp*ang_frecuency*cos(ang_frecuency*time);

    //Llamamos el método start_falling() del objeto ball para que este comience con la simulación de
    //la caída y rebote de la pelota.

    ball->start_falling(initial_speed);

    //Le damos un valor de 0.5 s^-1 a la variable gamma para comenzar a considerar la fricción de
    //la oscilación subamortiguada.

    gamma = 0.5;

    //Calculamos la nueva 'frecuencia angular' del movimiento subamortiguado, además del
    //nuevo valor de amp.

    ang_frecuency = sqrt(ang_frecuency*ang_frecuency - (b*b/(4*m*m)));
    amp = sqrt(initial_pos*initial_pos + pow(initial_speed + gamma*initial_pos, 2)/(ang_frecuency*ang_frecuency));

    //Ajustamos el valor de phase para lograr una transición continua entre el MAS y
    //la oscilación subamortiguada.

    if ((initial_speed + gamma*initial_pos) > 0) phase = asin(initial_pos/amp);
    else phase = M_PI - asin(initial_pos/amp);

    //Reiniciamos la variable time y actualizamos el estado del minijuego.

    time = 0;
    state++;
}

void Minigame::win() {

    //Este slot es llamado por la señal win de la clase Ball, y su propósito es notificar al usuario
    //de las recompensas que ha ganado y actualizar esta información en las variables de estado de
    //la partida.

    //Detenemos el movimiento subamortiguado que en este momento está describiendo la garra, además
    //de cerrarla de nuevo colocando claw_close en true y cambiando la imágen de esta.


    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw_move_timer->stop();
    claw_close = true;

    //Comenzamos con el cuerpo básico del mensaje que se le presentará al usuario.

    message = "      ¡MISIÓN CUMPLIDA!\n"
                      "\n"
                      "Recompensas obtenidas:\n";

    //Si le ha dado al objetivo en menos de dos rebotes de la pelota, le otorgamos
    //una vida extra, o en caso de que ya posea una, simplemente le informamos
    //que ya la tiene.

    if ((ball->get_rebounds() < 2) and !(*extra_life)) {
        message += "   Vida extra x 1\n";
        *extra_life = true;
    }
    else if (ball->get_rebounds() < 2) message += "   Ya tienes una vida extra\n";

    //Independientemente de la cantidad de rebotes que le haya tomado a la pelota chocar con el objetivo,
    //pues este método sólo se ejecuta si esto ocurre, le damos 2 rocas y 2 cargas de pegamente al
    //usuario, o en caso de que este ya cuente con algunas, le damos la cantidad adecuada para
    //que no supere cuatro cargas de roca o de pegamento.

    short won_rocks = fmin(4 - (*rocks_num), 2), won_glues = fmin(4 - (*glues_num), 2);

    //Como podemos ver en la sentencia anterior, si won_glues queda siendo 0, es porque el jugador
    //ya cuenta con cuatro cargas de pegamento, por lo cual se lo notificamos, o en caso
    //contrario, le informamos cuantas cargas ha obtenido.

    if (won_glues == 0) message += "   Ya tienes 4 pegamentos\n";
    else {
        message += "   Pegamento x ";
        message.push_back(char(won_glues + 48));
        message += "\n";
    }

    //Realizamos el proceso análogo para las cargas de rocas.

    if (won_rocks == 0) message += "   Ya tienes 4 rocas\n";
    else {
        message += "   Rocas x ";
        message.push_back(char(won_rocks + 48));
        message += "\n";
    }

    //Concluimos el mensaje informándole al usuario cuantos rebotes ha realizado la pelota antes
    //de chocar con el objetivo, además de un mensaje para que pueda continuar con el proceso
    //del minijuego.

    message += "\nNúmero de Rebotes: " + QString::number(ball->get_rebounds());

    if (two_players) message += "\nEnter o Espacio para continuar ->";
    else message += "\nEnter para continuar ->";

    //Actualizamos los valores hacia donde apuntan glues_num y rocks_num, mostramos el mensaje
    //que previametne elaboramos y actualizamos el estado del minijuego.

    (*glues_num) += won_glues;
    (*rocks_num) += won_rocks;

    information->display_message(389, 150, message);
    state++;
}

void Minigame::fail() {

    //Este método es utilizado para llevar a cabo el proceso correspondiente a cuando el jugador no ha
    //logrado superar el minijuego.

    //Detenemos el timer claw_move_timer y llamamos el método stop_timer() del objeto ball, para detener
    //el movimiento de la garra y la pelota.

    claw_move_timer->stop();
    ball->stop_timer();

    //Cerramos la garra cambiando su imágen y colocando la variable claw_close en true.

    claw->setPixmap(claw_pix[0]);
    claw->setOffset(-33, 0);
    claw_close = true;

    //Generamos un mensaje para indicarle al jugador que no ha superado el minijuego, este mensaje
    //varia ligeramente en el modo de dos jugadores.

    if (two_players) message = "                    ¡Tiempo Fuera!\nEnter o Espacio para continuar ->";
    else message = "         ¡Tiempo Fuera!\nEnter para continuar ->";

    //Le mostramos el mensaje previamente elaborado al usuario y actualizamos el estado del minijuego.

    information->display_message(389, 250, message);
    state++;
}

void Minigame::second_chance() {

    //Este método se encarga de realizar todos las modificaciones necesarias para comenzar con la
    //segunda oportunidad del minijuego.

    //Primero removemos la pelota de la escen y la eliminamos.

    removeItem(ball);
    delete ball;

    //Modificamos la posición del obejtivo para volverlo a colocar en una Y de 440 pixels y una X
    //aleatoria entre 200 y 580 pixels, inclusive.

    goal->setPos(rand()%381 + 200, 440);

    //Colocamos la garra en su posición inicial de nuevo.

    claw->setPos(389, 30);

    //Construimos y agregamos de nuveo a la escena el objeto ball.

    ball = new Ball(389, 103, goal->x(), goal->y());
    connect(ball, &Ball::win, this, &Minigame::win);
    addItem(ball);

    //Reinicimos las variables para el MAS de la garra.

    time = 0;
    gamma = 0;
    phase = 0;

    amp = 155;
    ang_frecuency = sqrt(k/m);

    //Elaboramos un mensaje para indicarle al usuario que se procederá con la segunda oportunidad del
    //minijuego; el mensaje varia ligeramente en el modo de dos jugadores.

    if (two_players) {
        message = "            Segunda Oportunidad\n"
                  "Segundo Jugador (Tecla Espacio)";
    }
    else message = "Segunda Oportunidad";

    //Mostramos el mensaje en pantalla, llamamos el método start() del objeto delay_timer para esperar
    //3 segundos antes de comenzar con la segunda oportunidad del minijuego, y por último actualizamos
    //el estado de este.

    information->display_message(389, 250, message);
    delay_timer->start(3000);
    state++;
}

void Minigame::next_state() {

    //Este slot es llamado por el timer delay_timer, y su propósito es comezanr como tal con el minijuego,
    //ya sea la primera o la segunda oportunidad.

    //Removemos el mensaje que se le esté mostrando al usuario en pantalla, llamamos el métod start() del
    //objeto claw_move_timer, y por último actualizamos el estado del minijuego.

    removeItem(information);
    claw_move_timer->start(50);
    state++;
}
