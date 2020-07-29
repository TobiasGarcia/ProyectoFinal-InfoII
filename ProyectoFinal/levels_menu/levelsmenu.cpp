#include "levelsmenu.h"

LevelsMenu::LevelsMenu(bool _two_players, short rocks_num, short glues_num, bool extra_life,
    short *_levels_waves, bool winner) : levels_waves(_levels_waves), two_players(_two_players) {

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/levels_menu/resources/images/levels_menu/backgorund.png")));

    //Utilizamos la variable state para indicar en cual estado del proceso de seleccion de un
    //nivel nos encontramos, los estados posibles son los siguientes: 0 para cuando el
    //usuario simplemente está moviendose por el menú de niveles y 1 para cuando el
    //usuario ya ha seleccionado un nivel. Debemos diferenciar estos dos estdos
    //pues en el primero debemos estar pendientes de la decisión del usuario,
    //mientras que en el segundo no le podemos permitir escoger más niveles
    //pues acabó de seleccionar uno, y estamos en el proceso de la
    //animación de la brújula u oscureciendo la pantalla para
    //comenzar con el nivel.

    state = 0;

    //Colocamos el HUD y las imágenes de los niveles en pantalla, además de
    //inicializar los objetos correspondientes al menú de niveles.

    display_hud(rocks_num, glues_num, extra_life, winner);

    display_levels();

    initialize_objects();

    //Hacemos la transición para retirar la pantalla negra.

    black_screen->change_opacity(false);
}

void LevelsMenu::display_hud(short rocks_num, short glues_num, bool extra_life, bool winner) {

    //Este método se encarga de colocar en pantalla el HUD, el cual esta conformado por las imágenes
    //para indica la cantidad de rocas y cargas de pegamento que posee el jugador, la imágen de un
    //salvavidas si se posee una vida extra, y un trofeo en caso de que en esta partida ya haya
    //superado todos los niveles.

    display_powers(rocks_num, glues_num);

    QGraphicsPixmapItem *graph_pix;
    if (extra_life) {
        graph_pix = new QGraphicsPixmapItem(QPixmap(":/levels_menu/resources/images/levels_menu/lifebuoy.png"));
        graph_pix->setPos(585, 49);
        addItem(graph_pix);
    }

    if (winner) {
        graph_pix = new QGraphicsPixmapItem(QPixmap(":/levels_menu/resources/images/levels_menu/trophy.png"));
        graph_pix->setPos(58, 47);
        addItem(graph_pix);
    }
}


void LevelsMenu::display_powers(short rocks_num, short glues_num) {

    //Este método coloca en pantalla las imágenes que indican la cantidad de rocas y cargas de pegamento
    //que posee el jugador.

    QGraphicsPixmapItem *power;
    for (short i = 0; i < rocks_num; i++) {
        power = new QGraphicsPixmapItem(QPixmap(":/power_ups/resources/images/power_ups/rock_power.png"));
        power->setPos(28 + 48*i, 546);
        addItem(power);
    }

    for (short i = 0; i < glues_num; i++) {
        power = new QGraphicsPixmapItem(QPixmap(":/power_ups/resources/images/power_ups/glue_power.png"));
        power->setPos(28 + 48*i, 503);
        addItem(power);
    }
}

void LevelsMenu::display_levels() {

    //Este método coloca en pantalla las imágenes de los niveles que puede escoger el jugador.

    short gap[] = {0, 1, 0, -1};
    QGraphicsPixmapItem *graph_pix;

    for (short i = 0; i < 4; i++) {

        //Colocamos los tres niveles y el tutorial.

        graph_pix = new QGraphicsPixmapItem(QPixmap(":/levels_menu/resources/images/levels_menu/level" + QString::number(i) + ".png"));
        graph_pix->setPos(389 + gap[i]*275, 299 + gap[(i + 1)%4]*230);
        graph_pix->setOffset(-78, -52.5);
        addItem(graph_pix);

        //En caso de que la imágen a colocar sea la de un nivel, pero este se encuentre bloqueado,
        //colocamos una imágen con un candado y opacidad de 0.7 sobre esta.

        if ((i != 0) and (levels_waves[i - 1] == 0)) {
            graph_pix = new QGraphicsPixmapItem(QPixmap(":/levels_menu/resources/images/levels_menu/locked.png"));
            graph_pix->setPos(389 + gap[i]*275, 299 + gap[(i + 1)%4]*230);
            graph_pix->setOffset(-78, -52.5);
            graph_pix->setOpacity(0.7);
            addItem(graph_pix);
        }
    }
}

void LevelsMenu::initialize_objects() {

    //Este método se encarga de construir todos los objetos que utiliza la clase LevelsMenu,
    //además de agregarlos a la escena.

    needle = new Needle;
    connect(needle, &Needle::finish, this, &LevelsMenu::needle_finish);
    addItem(needle);

    player1 = new Player(7, 2, false);
    addItem(player1);

    //En caso de estar en modo multijugador, contruimos y agregamos también
    //el objeto player2.

    if (two_players) {
        player2 =  new Player(7, 10, false, false);
        addItem(player2);
    }

    black_screen = new BlackScreen;
    connect(black_screen, &BlackScreen::finish, this, &LevelsMenu::black_screen_finish);
    addItem(black_screen);
}

LevelsMenu::~LevelsMenu() {
    delete needle;
    delete black_screen;

    delete player1;
    if (two_players) delete player2;
}

void LevelsMenu::keyPressEvent(QKeyEvent *event) {

    //Esta método se encarga de procesar las teclas presionadas en el teclado por el juegador.

    //El funcionamiento es análogo al de la reimplementación del método keyPressEvent() para
    //la clase Level, sólo que omitimos algunas funcionalidades: la plantilla para colocar
    //las rocas, la del menú de pausa y la del atributo freez de los objetos de la clase
    //Player.

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = true;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = true;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = true;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = true;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = true;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = true;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = true;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = true;

    //Recordemos que la tecla para el Enter cercano a las flechas corresponde a Qt::Key_Return,
    //Qt::Key_Enter es utilizado para el Enter del Numeric Keypad.

    else if ((event->key() == Qt::Key_Return)) add_fire_ball(player1->x(), player1->y());
    else if (two_players and (event->key() == Qt::Key_Space)) add_fire_ball(player2->x(), player2->y());
}

void LevelsMenu::keyReleaseEvent(QKeyEvent *event) {

    //Este método se encarga de procesar las teclas que los jugadores dejan de presionar.

    if (event->isAutoRepeat()) return;

    //El funcionamiento es análogo al de la reimplementación del método keyReleaseEvent() para
    //la clase Level, sólo que omitimos algunas funcionalidades: la plantilla para colocar
    //las rocas y la del atributo freez de los objetos de la clase Player.

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = false;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = false;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = false;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = false;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = false;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = false;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = false;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = false;
}

void LevelsMenu::add_fire_ball(short x, short y) {

    //Este método recibe una posición de la escena y construye y agrega un obejto
    //de la clase FireBall en dicho lugar, además de decidir si el usuario ha
    //seleccionado un nivel o el botón para salir.

    addItem(new FireBall(x, y));

    //Recordemos que si el valor de state es 0, significa que el usuario aún no ha escogido
    //el nivel, por lo cual revisamos si lo ha hecho mediante la fire ball colocada, o si
    //ha seleccionado el botón para salir.

    if (state == 0) {
        if (QRectF(314, 480.5, 149, 98).contains(x, y)) {

            //Revismoas si ha seleccionado el nivel tutorial, en cuyo caso le indicamos
            //al obejeto needle que apunte hacia abajo (2).

            needle->point_direction(2);
            level_num = 0;
            state++;
        }
        else if (QRectF(589, 250.5, 149, 98).contains(x, y) and (levels_waves[0] != 0)) {

            //Si no, revisamos si ha seleccionado el nivel 1, en cuyo caso le indicamos
            //al obejeto needle que apunte hacia la derecha (3).

            needle->point_direction(3);
            level_num = 1;
            state++;
        }
        else if (QRectF(314, 20.5, 149, 98).contains(x, y) and (levels_waves[1] != 0)) {

            //Si no, revisamos si ha seleccionado el nivel 2, en cuyo caso le indicamos
            //al obejeto needle que apunte hacia arriba (0).

            needle->point_direction(0);
            level_num = 2;
            state++;
        }
        else if (QRectF(39, 250.5, 149, 98).contains(x, y) and (levels_waves[2] != 0)) {

            //Si no, revisamos si ha seleccionado el nivel 3, en cuyo caso le indicamos
            //al obejeto needle que apunte hacia la izquierda (1).

            needle->point_direction(1);
            level_num = 3;
            state++;
        }
        else if (QRectF(626, 505, 92, 50).contains(x, y)) {

            //Si no, revisamos si ha seleccionado el botón para salir, en cuyo caso comenzamos el proceso
            //para terminar la selección del nivel y colocamos el valor de -1 en la variable level_num,
            //pues esto indica que se ha seleccionado el botón para salir.

            black_screen->change_opacity(true);
            level_num = -1;
            state++;
        }

        //Notemos que en cualquier caso dentro de los condicionales anteriores aumentamos el valor de la
        //variable state, sin embargo, no podemos colocar dicha instrucción fuera de los condicionales
        //porque se ejecutaria con tan sólo colocar una fire ball durante el estado 0, lo cual no
        //está de acuerdon con el diseño planeado.
    }
}

void LevelsMenu::needle_finish() {

    //Este slot es activado cuando el objeto needle concluye con la animación para apuntar en una
    //dirección en particualr, y su propósito es comenzar con el proceso para concluir con la
    //selección del nivel.

    black_screen->change_opacity(true);
}

void LevelsMenu::black_screen_finish() {

    //Este slot se encarga de emitir la señal level_selected para indicarle a la clase Game que
    //el usuario ha escogido un nivel, ha escogido el tutorial, o el botón para salir al menú
    //principal.

    emit level_selected(level_num);
}
