#include "level.h"

Level::Level(std::string path, bool _two_players, short level_num, short initial_wave, short *_rocks_num,
             short *_glues_num, bool *_extra_life, short *health, std::array<std::string, 9> *_terrain_matrix) :

             rocks_num(_rocks_num), glues_num(_glues_num), two_players(_two_players), extra_life(_extra_life),
             terrain_matrix(_terrain_matrix) {

    //La variable path es utilizada para poder llegar hasta la carpeta data, a la cual necesitan acceder
    //varios métodos de esta clase; la variable _two_players indica si el juego se encuentra en modo
    //de un solo jugador o multijugador; la variable level_num indica el número del nivel, o 0 en
    //caso de ser el tutorial; la variable initial_wave indica la oleada donde se retoma el
    //nivel; la variable _rocks_num es un puntero al número de rocas con las que cuenta el
    //el jugador en su partida; análogo en el caso de _glues_num pero para las cargas de
    //pegamento; el puntero _extra_life apunta a un bool que indica si el jugador cuenta
    //o no con una vida extra; health apunta a la salud de la base con que se retoma el
    //nivel; _terrain_matrix apunta a la matriz de terreno del nivel.

    //Los punteros son utilizados para que desde la clase Level se puedan modificar los valores de las
    //variables que definen el estado de la partida y que son administrados desde la clase Game.

    //Colocamos una escena de 780 x 600 piexeles, recordemos que como empezamos desde el pixel cero,
    //los últimos deben ser 779 y 599. Colocamos la imágen de fondo dependiendo del número del nivel,
    //las imágenes están en el siguiente formato: levelN_back.png, donde N es el número del nivel,
    //o cero si es el tutorial.

    setSceneRect(0, 0, 779, 599);
    setBackgroundBrush(QBrush(QPixmap(":/levels/resources/images/levels/level" + QString::number(level_num) + "_back.png")));

    initialize_flow_variables(level_num);

    get_level_dialogs(path, level_num);

    if (tutorial_level) set_up_tutorial(health);
    else set_up_level(path, level_num, initial_wave);

    display_hud(*health);

    initialize_objects(health);

    black_screen->change_opacity(false);
    next_dialog();
}

void Level::initialize_flow_variables(short level_num) {

    //Este método se encarga de inicializar las variables que necesita el nivel para
    //administrar su estado y flujo.

    action = 0;
    next = true;
    pause = false;
    gameover = false;
    dialoguing = true;

    //La variable tutorial_level nos indicará si el nivel es un nivel como tal, o es el tutorial,
    //este último corresponde al valor 0 de level_num.

    tutorial_level = (level_num == 0);
}

bool Level::get_level_dialogs(std::string path, short level_num) {

    //Al principio de cada nivel aparece la información de los enemigos nuevos que aparecerán,
    //dicha información se encuentra en un archivo .txt, este método se encarga de tomar esa
    //información y almacenarla dentro de la queue dialogs; utilizamos una queue porque
    //necesitamos mostrar el diálogo y posteriormente eliminarlo, pues ya no lo
    //necesitamos más.

    //Desde la clase Widget ya nos aseguramos que los archivos .txt existan, por lo cual los podemos
    //abrir con tranquilidad, aunque de todas formas implementamos esta función para que retorne
    //true en caso de poder abrir el archivo o false en caso contrario.

    std::string line, pre_dialog;
    bool next_line = true;

    //El formato para los archivos con los diálogos es el siguiente: levelN.txt, donde N corresponde
    //al número del nivel o a 0 para el tutorial.

    std::fstream file(path + "levels_dialogs/level" + char(level_num + 48) + ".txt", std::ios::in);

    if (file.is_open()) {

        //El formato de los diálogos dentro del .txt es:

        //DIRECTIVA\nDIALOGO\n\nDIRECTIVA\nDIALOGO\n\nDIRECTIVA\nDIALOGO...

        //Donde los tres puntos representan que pueden haber tantos bloques de DIRECTIVA\nDIALOGO\n
        //como se desee; DIRECTIVA es una de tres líneas de texto: la ruta de una imágen, esto significa que
        //al principio del diálogo se debe colocar la imágen especificada por dicha ruta; 'na', lo cual significa
        //que no debemos colocar ningúna imágen junto con el diálogo; 'acción', lo cual significa que debemos
        //esperar hasta que los jugadores realicen una acción determinada.

        //En los niveles sólo utilizamos la primera directiva, mientras que en el tutorial utilizamos
        //las tres.

        //Luego de la directiva sigue DIALOGO, el cual representa el texto del diálogo que se le mostrará al
        //usuario, puede contener saltos de línea y como mínimo debe ser de un caracter.

        while (next_line) {

            //Colocamos la directiva en el string pre_dialog.

            getline(file, pre_dialog);

            //Como tenemos seguridad de que el diálogo es de por lo menos un caracter, podemos utilizar
            //el siguiente ciclo while para tomar el resto de líneas.

            text = "";
            getline(file, line);

            while (next_line and (line != "")) {
                text.append(line);
                text.push_back('\n');
                if (!getline(file, line)) next_line = false;
            }

            //Le colocamos una indicación al usuario para que sepa que cuando termine de leer el texto
            //debe presionar la tecla Enter.

            text.append("\nPresiona Enter para continuar ->");

            //Utilizamos el arreglo de strings dialog, declarado como atributo de la clase Level pues
            //lo usaremos de nuevo dentro del método next_dialog(), para agregar la directiva y el
            //texto del diálogo a la queue dialogs.

            dialog[0] = pre_dialog;
            dialog[1] = text;
            dialogs.push(dialog);
        }
        file.close();
        return true;
    }
    else return false;
}

void Level::set_up_tutorial(short *(&health)) {

    //Este método se encarga de inicializar las variables que vienen por defecto en nullptr cuando se
    //está construyendo el tutorial en lugar de un nivel, además de inicializar el objeto terrain,
    //pues al ser el tutorial no se requiere cargar un terreno guardado previamente pues siempré
    //se comenzará el tutorial con un terreno limpio de objetos.

    terrain = new Terrain(this);

    //Como se menciona en la documentación de la clase Game, en el tutorial se pueden conseguir
    //objetos muy fácilmente, por lo cual no utilizamos las variables de estado de la partida,
    //sino que creamos unas que sólo se usarán para el tutorial, de esta forma el jugador no
    //podrá conservar los objetos que obtenga dentro del tutorial, aunque desde la clase
    //Game le permitimos conservar los que consiga en el minijuego.

    health = new short(1000);
    extra_life = new bool(true);
    rocks_num = new short(0);
    glues_num = new short(0);

    //Este timer lo inicializamos dentro de este método porque sólo es utilizado en el tutorial,
    //se encarga de revisar periodicamente si el jugador ha cumplido con la acción que se le
    //ha pedido.

    action_timer = new QTimer;
    connect(action_timer, &QTimer::timeout, this, &Level::check_action);

    //Como este es el tutorial, llamamos get_level_script() sin argumentos, pues esta es la versión
    //del método diseñada para llenar la queue script en el caso del tutorial.

    get_level_script();
}

void Level::set_up_level(std::string path, short level_num, short initial_wave) {

    //Este método se encarga de inicializar el objeto terrain a partir de la matriz terrain_matrix,
    //además de ajustar el valor a donde apunta rocks_num para que haya como máximo un total de
    //15 rocas entre las que están en el terreno y las que posee el jugador.

    terrain = new Terrain(this, terrain_matrix);
    while ((terrain->rocks_num + (*rocks_num)) > 15) (*rocks_num)--;

    //Agregamos los obejtos del terreno a la escena.

    display_terrain();

    //Como no utilizaremos el timer action_timer, pues este sólo es utilizado en el tutorial,
    //lo inicializamos en nullptr.

    action_timer = nullptr;

    //Llenamos la queue script con las instrucciones del nivel.

    get_level_script(path, level_num, initial_wave);
}

void Level::display_terrain() {

    //Agregamos a la escena todos los TerrainObjects que se encuentren dentro del objeto terrain.

    for (short i = 0; i < 9; i++) for (short j = 0; j < 13; j++) {
        if (terrain->tiles[i][j] != nullptr) addItem(terrain->tiles[i][j]);
    }
}

bool Level::get_level_script(std::string path, short level_num, short initial_wave) {

    //Este método se encarga de tomar las instrucciones que debe seguir el programa para desarrollar el nivel,
    //desde su respectivo archivo .txt.

    //Los enemigos, el momento en que aparecen, y los power ups de cada nivel, son programados por medio de
    //instrucciones dentro de un .txt, lo que permite que los niveles puedan ser modificados sin tener que
    //modificar el código del programa. Utilizamos algo así como un 'lenguaje de programación' demasiado
    //sencillo para indicarle al juego cuando debe colocar los enemigos, los power ups, etc. Cada
    //instrucción es una línea de texto del txt y se compone de dos partes, el primer caracter,
    //que indica el tipo de instrucción, y el resto de caracteres de la línea, los cuales
    //sirven como argumento según el tipo de instrucción; los tipos (el primer caracter
    //de la línea) y sus argumentos son los siguientes:

    //Wn:         Indica que se comienza la oleada n (desde la 1 hasta la 5).

    //Sn:         Indica que se debe hacer una pausa de n milisegundos antes de proseguir con la siguiente
    //            instrucción del archivo de text.

    //Eddddd... : Indica que se deben agregar enemigos, cada d es un dígito desde el 0 hasta el 8 y corresponde
    //            al tipo de enemigo que se debe agregar (para conocer los tipos ver documentación del método
    //            define_personality() de la clase Enemy), los '...' no van en realidad, solo representan
    //            que se pueden colocar tantos enemigos como se desee, uno por cada dígto; los enemigos
    //            van siendo agregados a la escena cada 0.5 segundos en caso de que se deba agregar
    //            varios, y no se prosigue con la siguiente instrucción del .txt hasta agregarlos
    //            a todos.

    //P:          Indica que se debe agregar un power up; el tipo de power up es decidido desde el método
    //            add_power_up(), basándose en la situación en la que se encuentre el jugador, brindándole
    //            una opción que se ajuste a sus necesidades del momento.

    //Fb:         Indica que se ha terminado la oleada y que se debe esperar a que el jugador acabe con todos
    //            los enemigos de la escena para darla por superada; no se continua con la siguiente instrucción
    //            del archivo hasta que se elimine a todos los enemigos de la pantalla. El argumento b puede
    //            tomar dos valores, 0 en caso de que sea una oleada cualquiera, o 1 en caso de que sea la
    //            última oleada del nivel.

    //L           Esta instrucción indica que el nivel ha sido superado, y debe ser colocada siempere después de
    //            la instrucción F1 (ver descripción de la instrucción anterior).

    //Los archivos .txt con las instrucciones de cada nivel siguen el siguiente formato: bleveln, donde n
    //corresponde al número del nivel, y b es un dígito que puede ser 0 o 1, 0 para indicar que son las
    //instrucciones del modo de un solo jugador y 1 para las instrucciones del modo multijugador.

    //Como en el .txt se encuentran las instrucciones de todas las oleadas, en caso de que el jugador retome
    //el nivel en una oleada diferente a la primera, las instrucciones de las oleadas anteriores no
    //interesan, por lo cual no las cargamos dentro de la queue script, la cual se encargará de
    //almacenar todas las instrucciones del nivel, para esto utilizamos la variable
    //current_game, la cual se coloca en true cuando se encontró la instrucción
    //Wn, donde n corresponde a la oleada donde se comienza el nivel, de esta
    //forma omitimos todas las instrucciones previas.

    //Utilizamos una queue pues las instrucciones las debemos leer y luego eliminar, pues no las necesitamos
    //más luego de haberlas procesado.

    //Recordemos que la existencia de los archivos de texto ya fue verificada dentro del método check_txt_files()
    //de la clase Widget, aunque de todas formas programamos este método para que retorne true en caso de poder
    //abrir el archivo correctamente, o false en caso contrario.

    bool current_game = false;
    std::fstream file(path + "levels_scripts/" + (two_players?'1':'0') + "level" + char(level_num + 48) + ".txt", std::ios::in);

    if (file.is_open()) {

        //Almacenamos la instrucción dentro del string instruction.

        while (getline(file, instruction)) {

            //Si ya current_game es true, significa que todas las instrucciones que leamos nos interesan,
            //por las agregamos a la queue script.

            if (current_game) script.push(instruction);

            //En caso de que aún no hayamos encontrado las instrucciones de la oleada donde se retomará
            //el nivel, verificamos si la que acabamos de leer corresponde a Wn, donde n es la oleada
            //en la cual se retoma el nivel.

            else if ((instruction[0] == 'W') and ((short(instruction[1]) - 48) == initial_wave)) {

                //Y en caso afirmativo, la agregamos a la queue script y colcamos current_game en true
                //para continuar agregando las instrucciones posteriores.

                script.push(instruction);
                current_game = true;
            }
        }
        file.close();
        return true;
    }
    else return false;
}

bool Level::get_level_script() {

    //Este es un método sobrecargado; es llamado dentro del método set_up_tutorial(),
    //pues el tutorial no cuenta con una archivo de instrucciones.

    script.push("L");
    return true;
}

void Level::initialize_objects(short *health) {

    //Este método se encarga, directamente o llamando otros métodos, de reservar memoria para todos los objetos
    //que utiliza la clase Level, a excepción del objeto terrain, pues este es inicializado dentro del
    //método set_up_tutorial() o set_up_level() dependiendo del caso.

    initialize_timers();
    initialize_template();
    initialize_base(health);

    //El objeto fire_ball es utilizado para modelar las bolas de fuego con que los jugadores
    //atacan a los enemigos.

    fire_ball = nullptr;
    power_up = nullptr;
    enemie = nullptr;

    //El propósito del objeto ghost_rock es descrito dentro del método add_rock().

    ghost_rock = new QGraphicsRectItem(-10, -10, 80, 80);

    //El objeto information es utilizado para colocar texto en la escena bajo un formato predefinido
    //descrito dentro de la documentación de la clase Information.

    information = new Information(this);

    //Construimos el objeto player1 que será controlado por el primer jugador; si la variable two_players
    //es true también construimos el objeto que controlará el segundo jugador. Tenemos que colocar
    //su atributo freez en true para que los jugadores no se puedan mover hasta que se terminen
    //los diálogos del principio del nivel.

    player1 = new Player(5, 5, true);
    player1->freez = true;

    if (two_players) {
        player2 =  new Player(5, 7, true, false);
        player2->freez = true;
    }

    //El objeto black_screen es simplemente un rectángulo negro que cubre toda la pantalla y cambia
    //su opacidad según le indiquemos, es utilizado para dar un buen efecto visual cuando se va a
    //cambiar a otra escena.

    black_screen = new BlackScreen;
    connect(black_screen, &BlackScreen::finish, this, &Level::finish_level);
    addItem(black_screen);
}

void Level::initialize_timers() {

    //Este método inicializa y conecta los timers que usa la clase Level,
    //además de inicializar también el arreglo active_timers.

    //El timer action_timer sólo es utilizado en el tutorial,
    //por lo cual no lo inicializamos aquí sino dentro del
    //nétodo set_up_tutorial().

    instructions_timer = new QTimer;
    connect(instructions_timer, &QTimer::timeout, this, &Level::next_instruction);

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Level::finish_delay);

    freez_timer = new QTimer;
    freez_timer->setSingleShot(true);
    connect(freez_timer, &QTimer::timeout, this, &Level::defrost);
}

void Level::initialize_template() {

    //Este método inicializa la plantilla que se coloca para indicarle al jugador en qué lugares del mapa
    //puede colocar rocas, para más información sobre la plantilla y la variable template_on consultar la
    //documentación de la reimplementación del método keyPressEvent() para esta clase.

    template_on = 0;

    power_template = new QGraphicsPixmapItem;
    power_template->setPixmap(QPixmap(":/levels/resources/images/levels/available.png"));
    power_template->setPos(180, 120);
    power_template->setOpacity(0.6);
    power_template->setZValue(0);
}

void Level::initialize_base(short *health) {

    //Este método inicializa los objetos lifebuoy y base.

    //El objeto lifebuoy es un pixmap con una imágen de un salvavidas, el cual se coloca sobre la imágen
    //de la base cuando el jugador cuenta con una vida extra.

    lifebuoy = nullptr;
    if ((*extra_life)) {

        lifebuoy = new QGraphicsPixmapItem(QPixmap(":/base/resources/images/base/lifebuoy_small.png"));
        lifebuoy->setOffset(-30, -30);
        lifebuoy->setPos(390, 270);

        //Le colocamos ese Z value para que esté por encima de la base, pues esta se agrega después a la
        //escena, pero para que al mismo tiempo esté por debajo de los enemigos.

        lifebuoy->setZValue(2);
        addItem(lifebuoy);
    }

    base = new Base(health_bar, health, extra_life, lifebuoy, tutorial_level);
    connect(base, &Base::no_health, this, &Level::no_health);
    addItem(base);
}

void Level::display_hud(short initial_health) {

    //Este método se encarga de inicializar y colocar en pantalla todos los objetos
    //que forman el HUD del juego.

    //Un rectángulo gris que se coloca detrás de la barra de salud de la base para
    //que no se quede viendo el fondo del nivel cuando esta se reduzca.

    QGraphicsRectItem *rect = new QGraphicsRectItem(221, 544, 337, 51);
    rect->setBrush(QColor(86, 86, 86));
    rect->setZValue(5);
    addItem(rect);

    //La barra de salud de la base, la cual es un rectángulo azul; notemos que su
    //longitud inicial depende de la salud de la base cuando se inicial el nivel.

    health_bar = new QGraphicsRectItem(221, 544, 337*(initial_health/1000.0), 51);
    health_bar->setBrush(QColor(54, 104, 195));
    health_bar->setPen(QColor(54, 104, 195));
    health_bar->setZValue(5);
    addItem(health_bar);

    //Agregamos una imágen para la barra naranja donde se encuentra la barra de salud
    //de la base y la cantidad de power ups que posee el jugador.

    QGraphicsPixmapItem *pix_map = new QGraphicsPixmapItem;
    pix_map->setPixmap(QPixmap(":/levels/resources/images/levels/hud.png"));
    pix_map->setPos(0, 540);
    pix_map->setZValue(5);
    addItem(pix_map);

    //Al jugador se le indica la cantidad de power ups que posee mediante unas pequeñas imágenes que se agregan
    //al HUD, estas imágenes están cargadas siempre dentro de los arreglos de pixmaps rock_powers y glue_powers,
    //y lo que hacemos es agregarlas o removerlas de la escena en función de la cantidad de power ups que se
    //posea en el momento.

    rock_powers = new QGraphicsPixmapItem[4];
    for (short i = 0; i < 4; i++) {

        rock_powers[i].setPixmap(QPixmap(":/power_ups/resources/images/power_ups/rock_power.png"));
        rock_powers[i].setPos(20 + 50*i, 555);
        rock_powers[i].setZValue(5);

        if (i < (*rocks_num)) addItem(rock_powers + i);
    }

    glue_powers = new QGraphicsPixmapItem[4];
    for (short i = 0; i < 4; i++) {

        glue_powers[i].setPixmap(QPixmap(":/power_ups/resources/images/power_ups/glue_power.png"));
        glue_powers[i].setPos(730 - 50*i, 555);
        glue_powers[i].setZValue(5);

        if (i < (*glues_num)) addItem(glue_powers + i);
    }
}

Level::~Level() {
    delete base;
    delete player1;
    delete terrain;
    delete ghost_rock;
    delete health_bar;
    delete freez_timer;
    delete information;
    delete delay_timer;
    delete black_screen;
    delete power_template;
    delete instructions_timer;

    delete[] rock_powers;
    delete[] glue_powers;

    if (tutorial_level) {
        delete extra_life;
        delete rocks_num;
        delete glues_num;
    }

    if (action_timer != nullptr) delete action_timer;
    if (lifebuoy != nullptr) delete lifebuoy;
    if (power_up != nullptr) delete power_up;
    if (two_players) delete player2;

    for (short i = 0; i < enemies.size(); i++) {
        if (enemies[i]->scene() != nullptr) removeItem(enemies[i]);
        delete enemies[i];
    }
}

void Level::next_dialog() {

    //Este método se encarga de mostrar en pantalla el diálogo que se encuentre al frente de la queue
    //dialogs, para posteriormente eliminarlo de esta; en el caso de que dialogs ya se encuentre
    //vacío, comenzamos el nivel o terminamos el tutorial, pues esté último acaba cuando se
    //terminan todos los diálogos.

    //El método change_opacity() de la clase BlackScreen, emite una señal cuando termina de aumentar
    //la opacidad del rectángulo negro, la cual va conectada al slot finish_level() que se encarga
    //de terminar el nivel o el tutorial.

    if (dialogs.empty() and tutorial_level) black_screen->change_opacity(true);

    //En caso de que ya se hayan acabado de mostrar todos los diálogos y no sea el tutorial,
    //comenzamos el nivel mediante el método start_level().

    else if (dialogs.empty()) start_level();
    else {

        //En caso de que aún queden diálogos por procesar, lo almacenamos en el arreglo dialog y
        //procedemos a realizar lo que se indica en la directiva de su primer elemento, para
        //más información sobre las directivas de los diálogos consultar la documentación
        //del método get_level_dialogs().

        dialog = dialogs.front();

        if (dialog[0] == "na") information->display_message(389, 20, QString::fromUtf8(dialog[1].c_str()));
        else if (dialog[0] == "acción") wait_for_user_action();
        else information->display_message(389, 135, QString::fromUtf8(dialog[1].c_str()), QString::fromUtf8(dialog[0].c_str()));

        dialogs.pop();
    }
}

void Level::start_level() {

    //Este método es utilizado para comenzar el nivel luego de que los diálogos
    //se le han presentado al usuario.

    //Para que el usuario pase al siguiente diálogo, el programa le indica que debe
    //presionar la tecla Enter, la cual también es utiliza para atacar, por lo tanto
    //utilizamos la variable dialoguing para indicar cuando hay un diálogo en la
    //pantalla. Puesto que ya no hay más diálogos colocamos la variable
    //dialoguing en false.

    dialoguing = false;

    //Colocamos el atributo freez del objeto player1 en false para permitir que el
    //jugador comience a controlarlo mediante el teclado, además de agregalo a la
    //escena; realizamos el mismo proceso para el objeto player2 en caso de que
    //la variable two_players se encuentre en true.

    player1->freez = false;
    addItem(player1);
    if (two_players) {
        player2->freez = false;
        addItem(player2);
    }

    //Llamamos el método start() de instructions_timer para que comience a llamar al
    //slot next_instruction(), el cual procesa las instrucciones del nivel.

    instructions_timer->start(500);

    //Como el timer instructions_timer no ejecuta el slot justo cuando comienza la
    //cuenta regresiva, lo llamamos manualmente.

    next_instruction();
}

void Level::wait_for_user_action() {

    //Este método se encarga de colocar algunos objetos en la escena y comenzar a esperar
    //hasta que el jugador cumpla con determinada acción. Sólo es llamado en el tutorial.

    //Colocamos el atributo freez del primer jugador en false para que se pueda mover
    //libremente; hacemos lo mismo para el segundo jugdaor en caso de que two_players
    //sea true.

    player1->freez = false;
    if (two_players) player2->freez = false;

    //La variable action nos ayudará a saber en cual parte del tutorial estamos, hay tres momentos donde
    //necesitamos esperar a que el jugador realice una acción, las cuales son las siguientes según el
    //valor de action:

    if (action == 0) {

        //0: Agregamos dos enemigos del tipo 0 y esperamos a que el jugador los derrote.

        information->display_message(389, 15, "Presiona Enter para atacar, o en el\n"
                                              "caso del segundo jugador Espacio.");

        //Agregamos el objeto player1 a la escena, al igual que player2 en caso de que
        //two_players sea true.

        addItem(player1);
        if (two_players) addItem(player2);

        //Añadimos los dos enemigos de tipo 0.

        add_enemie(0);
        add_enemie(0);
    }
    else {

        //1: Agregamos un power up de pegamento que no desaparece con el tiempo como lo haría durante un nivel,
        //   esperamos a que el jugador lo destruya y coloque una carga de pegamento en cualquier lugar del mapa.

        //2: Agreamos un power up de roca que tampoco desaparece con el tiempo, esperamos a que el jugador lo
        //   destruya y coloque una roca.

        //Utilizamos glue para diferenciar entre cuando action es 1 o 2, esto es preferible a utilizar un
        //else if pues los códigos de ambos casos son muy similares.

        bool glue = (action == 1);

        if (glue) information->display_message(389, 15, "Destruye la cápsula y coloca una carga de pegamento\n"
                                                        "con Shift Derecho, o en el caso del segundo jugador,\n"
                                                        "la tecla B.");
        else information->display_message(389, 15, "Destruye la cápsula y coloca una roca con la tecla Borrar,\n"
                                                   "la que está arriba del Enter, o en el caso del segundo,\n"
                                                   "jugador la tecla V.");

        power_up = new PowerUp(rand()%4, (glue?3:4));
        connect(power_up, &PowerUp::give_power, this, &Level::give_power);
        addItem(power_up);
    }

    //Colocamos dialoguing en false pues tenemos que permitirle al primer jugador atacar con la tecla Enter,
    //además de llamar el método start() de action_timer, el cual activa el slot check_action(), el cual se
    //encarga de revisar si el jugador ya ha cumplido con la acción solicitada.

    dialoguing = false;
    action_timer->start(50);
}

void Level::check_action() {

    //Este slot se encarga de revisar si el jugador ha cumplido con la acción solicitada
    //por el tutorial.

    //Si action es 0, debemos revisar si ya ha eliminado a los enemigos; si action es 1, debemos revisar
    //que ya haya destruido el power up, es decir, el objeto power_up apunta a nullptr, además de que
    //haya colocado por lo menos una carga, es decir, *glues_num es menor a 4; el caso en que action
    //es 2 es análogo al de action en 1, pero con rocas en lugar de pegamento.

    if (((action == 0) and enemies.isEmpty()) or
        ((action == 1) and (power_up == nullptr) and ((*glues_num) < 4)) or
        ((action == 2) and (power_up == nullptr) and ((*rocks_num) < 4))) {

        //En cualquier caso debemos realizar el mismo proceso, detener action_timer para no revisar más,
        //colocar el atributo freez del objeto player1 en true para que no se pueda mover más (el del
        //objeto player2 también en caso de que two_players esté en true), colocar dialoguing en true
        //pues se va a continuar con los diálogos y necesitamos que la tecla enter ahora sea para
        //avanzar entre esto en lugar de lanzar fire balls, aumentamos action para que la proxima
        //vez que se llame al método wait_for_user_action() se proceda con la siguiente acción,
        //y por último llamamos al método next_dialog() para continuar con el tutorial.

        action_timer->stop();

        player1->freez = true;
        if (two_players) player2->freez = true;

        dialoguing = true;
        action++;

        next_dialog();
    }
}

void Level::next_instruction() {

    //Este método se encarga de procesar las instrucciones que debe seguir el nivel.

    //Si estamos en el menú de pausa retornamos.

    if (pause) return;

    //Almacenamos la primera intrucción de la queue script en el string instruction,
    //esto es pues algunos de los siguientes métodos extraen información de este
    //string.

    instruction = script.front();

    if (instruction[0] == 'W') start_wave(short(instruction[1]) - 48);
    else if (instruction[0] == 'S') wait(stoi(instruction.substr(1)));
    else if (instruction[0] == 'E') add_enemies(instruction);
    else if (instruction[0] == 'P') add_power_up();
    else if (instruction[0] == 'F') finish_wave();
    else level_completed();

    //Si luego de ejecutar alguno de los métodos anteriores, la variable next es true,
    //es porque debemos continuar con la siguienteinstrucción, en caso de que sea
    //false, es porque debemos mantenernos en la misma instrucción hasta que se
    //finalice un proceso.

    if (next) script.pop();
}

void Level::start_wave(short wave) {

    //Este método se encarga de anunciar al jugador que se llevará a cabo
    //la oleada número wave.

    information->display_message(389, 194, "Oleada " + QString::number(wave));
    information->set_display_time(3000);

    //El timer delay_timer lo utilizamos para poder detener el timer instructions_timer, y así
    //detener el procesamiento de instrucciones del nivel, para poder reanudarlo en un período
    //de tiempo determinado.

    delay_timer->start(3000);
    instructions_timer->stop();
}

void Level::wait(short mili) {

    //Este método detiene el procesamiento de instrucciones del nivel por un breve
    //período de tiempo.

    delay_timer->start(mili);
    instructions_timer->stop();
}

void Level::add_enemies(const std::string &instruction) {

    //Este método añade los enemigos indicados en el string instruction.

    //Como preferí añadir los enemigos cada 0.5 segundos y no todos de golpe, la primera vez que se ejecuta
    //este método, cuando se desea añadir varios enemigos (next se encuentra en true), colocamos next en
    //false para que el método next_instruction() no elimine esta instrucción hasta haberlos agregado a
    //todos.

    if (next) {

        //Almacenamos la cantidad de enemigos que debemos agregar, la cual corresponde a la cantidad de
        //caracteres del string intruction menos uno, dentro de la variable max_enemies, y colocamos
        //enemie_count para comenzar la cuenta de cuantos hemos colocado.

        max_enemies = instruction.size() - 1;
        enemie_count = 0;
        next = false;
    }

    //Añadimos un enemigo del tipo indicado por el string instruction mediante el método add_enemie(),
    //y aumentamos el contador de enemigos colocados.

    add_enemie(short(instruction[enemie_count + 1]) - 48);
    enemie_count++;

    //En caso de que ya hayamos colocado a todos los enemigos, colocamos next en true para que el
    //método next_instruction() prosiga con la siguiente instrucción.

    if (max_enemies == enemie_count) next = true;
}

void Level::add_power_up() {

    //Este método se encarga de construir y colocar el objto power_up en la escena.

    //El tipo de power up es determinado por la situació actual del jugador:

    //NOTA: En todos los condicionales siempre utilizamos rand()%4, esto es para definir
    //la trayectoria que seguirá la cápsula del power up, la cual es independiente del
    //poder extra que este otorga.

    //En caso de que hayan más de 3 enemigos en la pantalla, le proporcionamos el power up
    //para congerlalos o el que golpea a todos los enemigos.

    if (enemies.size() > 3) power_up = new PowerUp(rand()%4, rand()%2);

    //Si no, en caso de que la salud de la base sea inferior a 500, le proporcionamos el power up
    //que le restaura salud a la base.

    else if (base->get_health() < 500) power_up = new PowerUp(rand()%4, 2);

    //Si no, le proporcionamos el power up que otorga cargas de rocas o el que otorga cargas de
    //pegamento; utilzamos el rand()%3 para darle una probabilidad de 66.67% al que otroga
    //rocas y 33.33% al que otorga pegamentos.

    //Obsérvese que independientemente de el rand()%3, en caso de que ya hayan 15 rocas en el
    //terreno otorgamos el power up de pegamento.

    else if (rand()%3 and (terrain->rocks_num < 15)) power_up = new PowerUp(rand()%4, 4);
    else power_up = new PowerUp(rand()%4, 3);

    //Conectamos las señales del power up con sus respectivos slots, lo agregamos a la escena
    //y le indicamos que debe autodestruirse en 15 segundos, este es el tiempo que posee el
    //jugador para poder destruirlo y obtener el power up, pero pasado este tiempo el power
    //up se autodestruye y, naturalmente, no otorga ningún poder extra al jugador.

    connect(power_up, &PowerUp::give_power, this, &Level::give_power);
    connect(power_up->display_timer, &QTimer::timeout, this, &Level::remove_power_up);
    power_up->display_timer->start(15000);
    addItem(power_up);
}

void Level::finish_wave() {

    //Este método se encarga de esperar a que el jugador elimine a todos los enemigos restantes
    //en la pantalla, para dar por superada la oleada y emitir la señal para guardar la partida.

    //La primera vez que entramos a este método cuando se lee la instrucción del final de la oleada,
    //colocamos next en false para no continuar procesando instrucciones hasta que se elimine
    //a todos los enemigos.

    if (next) next = false;
    else if (enemies.isEmpty()) {

        //Cuando se haya eliminado a todos los enemigos, colocamos next en true para proseguir
        //con el procesamiento de insrucciones del nivel.

        next = true;

        //Como se describe en la documentación del método get_level_script(), si el segundo caracter
        //de la instrucción para finalizar la oleada se encuentra un 0, es porque aún no se ha
        //superado la oleada final, por lo cual actualizamos la información de la matriz del
        //terreno que administra la clase Game con la información del objeto terrain, para
        //posteriormente emitir la señal para guardar la partida e indicarle al jugador
        //que la oleada ha finalizado.

        if (instruction[1] == '0') {

            terrain->update_terrain_matrix(terrain_matrix);
            emit update_level_progress(2);

            information->display_message(389, 194, "¡Fin de la Oleada!");
            information->set_display_time(3000);
        }

        //En el caso de que se haya finalizado la última oleada del nivel, le indicamos al jugador
        //que el nivel ha finalizado, pero aún no guardamos la partida pues aún falta que el
        //jugador participe del minijuego para obtener recompensas.

        else information->display_message(389, 194, "Nivel Superado");

        //Esperamos un momento mediante el timer delay_timer, para finalmente procesar la instrucción
        //'L', la cual como se especificó dentro de la documentación del método get_level_script(),
        //se coloca siempre después de la instrucción de finalización de la última oleada ('F1').

        delay_timer->start(3000);
        instructions_timer->stop();
    }
}

void Level::level_completed() {

    //Este método se encarga de llamar el método change_opacity(), del objeto black_screen,
    //pues es mediante el slot finish_level() que se concluye el nivel como tal.

    black_screen->change_opacity(true);
    instructions_timer->stop();
}

void Level::add_enemie(short type) {

    //Este método se encarga de construir y añadir un enemigo a la escena del tipo type.

    //Elegimos aleatoriamente un lado por donde aparecerá el enemigo, 0 para arriba,
    //1 para izquierda, 2 para abajo y 3 para derecha.

    short i, j, side = rand()%4;

    //Tomamos una casilla aleatoria del lado seleccionado que se encuentre en la región
    //donde aparecen los enemigos (fuera de la pantalla), para más información consultar
    //la documentación del método set_targets() de la clase Enemy.

    if (side%2) {
        i = rand()%9;
        j = 7*side - 8;
    }
    else {
        i = 5*side - 1;
        j = rand()%13;
    }

    //Construimos el objeto enemie dependiendo del valor de la variabl type; la memoria ocuapada
    //por el objeto enemie es liberada cuando este es eliminadopor el jugador.

    if (type == 8) {

        //En caso de que el enemigo a construir sea de la clase Vulture, debemos realizar una conexión
        //extra pues esta clase posee una señal particular.

        enemie = new Vulture(this, terrain, enemies.size());
        connect(enemie, &Enemy::vulture_hit, base, &Base::vulture_hit);
    }
    else if (type == 7) enemie = new Mole(this, terrain, enemies.size());
    else if (type == 6) enemie = new Chamaleon(i, j, this, terrain, enemies.size());
    else if (type == 5) enemie = new Owl(i, j, this, terrain, enemies.size());
    else if (type == 4) enemie = new Porcupine(i, j, this, terrain, enemies.size());
    else if (type == 3) enemie = new Snail(i, j, this, terrain, enemies.size());

    //Los enemigos anteriores son enemigos especiales, pues describen o interactuan diferente
    //a como lo hace un objeto de la clase Enemy, pero los enemigos de tipo 0, 1 o 2, son
    //los enemigos básicos del juego, por lo cual basta con la sentencia del sigueinte
    //else para construirlos.

    else enemie = new Enemy(i, j, type, this, terrain, enemies.size());

    //Llamamos el método make_connections() que realiza las conexiones entre las señales y slots
    //necesarios para el correcto funcionamiento de la clase Enemy.

    make_connections(enemie);

    //Agregamos el objeto enemie a la QList enemies, y posteriormente lo agregamos a la escena.

    enemies.append(enemie);
    addItem(enemie);
}

void Level::make_connections(Enemy *enemy) {

    //Este método se encarga de reaizar las conexiones de las señales del objeto
    //enemie con sus respectivos slots.

    connect(enemy, &Enemy::first_bite, base, &Base::bitten);
    connect(enemy->bite_timer, &QTimer::timeout, base, &Base::bitten);
    connect(enemy, &Enemy::remove_enemy, this, &Level::remove_enemy);
    connect(this, &Level::update_index, enemy, &Enemy::update_index);
}

void Level::give_power(short power_type) {

    //Este slot se encarga de otorgar el poder extra del respectivo power up al jugador,
    //cuando este lo ha destruido.

    //Este slot es activado justo antes de que, desde el método test_collisions() de la clase
    //FireBall, el power up sea eliminado mediante delete, por lo cual en este slot podemos
    //tranquilamente apuntar power_up a nullptr, pues test_collisions() ya se encargará de
    //liberar la memoria que ocupaba el objeto power_up.

    power_up = nullptr;

    //La correspondencia entre los tipos de power up y lo que hacen es la siguiente:

    //0: Golpea a todos los enemigos.
    //1: Congela a todos los enemigos por un período de tiempo.
    //2: Restaura un poco la salud de la base.
    //3: Otorga cuatro cargas de pegamento.
    //4: Otroga cuatro o menos cargas de rocas, esto depende de cuantas hay en el terreno
    //   y cuantas posee el jugador, pues nunca podemos permitir que la suma de estas dos
    //   cantidades supere al número 15, pues esto interferiría con la forma en que los
    //   enemigos se mueven.

    //En caso de que el tipo de power up sea 0, llamamos el métdo hit_all_enemies().

    if (power_type == 0) hit_all_enemies();
    else if (power_type == 1) {

        //Para congelar a los enemigos utilizamos el método set_enemies_freez() evaluado en true,
        //y llamamos el método start() de freez_timer para que los descongele pasados 5 segundos.

        set_enemies_freez(true);
        freez_timer->start(5000);
    }

    //En caso de que el tipo de power up sea 0, incrementamos la salud de la base en 500 mediante
    //le método increase_health() de la clase Base.

    else if (power_type == 2) base->increase_health(500);
    else if (power_type == 3) {

        //En caso de otorgar cargas de pegamento al jugador, nos debemos asegurar de que no vaya
        //a quedar con más de 4, puesto que este es el máximo, para lo cual utilizamos le
        //siguiente cilo while.

        while ((*glues_num) < 4) {
            (*glues_num)++;
            addItem(glue_powers + (*glues_num) - 1);
        }
    }
    else {

        //En caso de otorgar cargas de rocas, el proceso es análogo a las cargas de pegamneto,
        //sólo que también debemos procurar que la suma entre las rocas que posee el jugador
        //las rocas del terreno nunca supere el valor de 15.

        while (((*rocks_num) < 4) and ((terrain->rocks_num + (*rocks_num)) < 15)) {
            (*rocks_num)++;
            addItem(rock_powers + (*rocks_num) - 1);
        }
    }
}

void Level::hit_all_enemies() {

    //Este método se encarga de bajarle salud a todos los enemigos de la pantalla.

    short index = 0, size = enemies.size();
    while (index < size) {

        //En el caso particular del topo, como este no aparece inmediatamente sino que coloca un
        //agujero y pasado un breve período de tiempo sí aparace en pantalla, debemos utilizar
        //el siguiente condicional para asegurarnos de golpearlo sólo si ya ha aparecido en
        //el campo visual del jugador.

        if (enemies.at(index)->get_type() == 7) {
            Mole *mole = dynamic_cast<Mole*>(enemies.at(index));
            if (mole->dig_timer->isActive()) {
                index++;
                continue;
            }
        }

        //En este punto se enuentra un detalle que requiere de mucha atención, mientras recorremos la
        //QList enemies y vamos reduciendo su salud, puede suceder de que algunos enemigos sean
        //derrotados en el proceso, por lo cual algunos índices de la QList enemies se reducen,
        //y si hubieramos empleado un ciclo for para esta tarea tendríamos un problema
        //porque nos estaríamos saltando enemigos, ya que el contado del ciclo siempre
        //aumentaría, por lo cual, en caso de que el enemigo golpeado hay sido
        //derrotado, es decir, la longitud de enemies se ha reducido,
        //disminuimos nuestra variable size en lugar de aumentar
        //index, o mantenemos size en el mismo valor y
        //aumentamos la variable index en caso
        //contrario.

        enemies.at(index)->reduces_health(300);
        if (size == enemies.size()) index++;
        else size--;
    }
}

void Level::set_enemies_freez(bool freez) {

    //Este método se encarga de congelar a todos los enemigos de escena.

    //Notemos que en el caso de que hayan enemigos que estén reduciendo la salud de la base,
    //no solo debemos detener su movimietno sino tamibén sus golpes, por lo cual utilizamos
    //el método set_vulnerable() de la clase Base para colocar la base invulnerable
    //durante todo el período de congelamiento, o vulnerable cuando este termine.

    base->set_vulnerable(!freez);
    for (short i = 0; i < enemies.size(); i++) enemies.at(i)->set_freez(freez);

    //Este caso es mucho más sencillo que el método hit_all_enemies(), pues aquí, a medida que recorremos
    //la QList enemies, su tamaño no varía, además de que tampoco debemos omitir al topo, puesto que
    //cuando este si era omitido dentro de este método al igual que en hit_all_enemies(), se veía
    //muy extraño desde la perspectiva del jugador ver a todos los enemigos congelados pero al
    //topo salir de su agujero tranquilamente, por lo cual decidí congelarlo a él también
    //así este no haya salido de su agujero aún.
}

void Level::remove_power_up() {

    //Este slot es activado cuando el timer display_timer, de la clase PowerUp, lanza la señal timeout,
    //y su propósito es eliminar al power up de la pantalla pues el jugador no ha alcanzado a
    //acertarle nigún ataque.

    //El condicional es sólo por precaución, pues puede suceder que el jugador destruya el power up
    //justo en el momento en que debe ser eliminado por este slot.

    if (power_up != nullptr) {
        delete power_up;
        power_up = nullptr;
    }
}

void Level::remove_enemy(short list_index) {

    //Este slot es activado cuando un enemigo es derrotado, y su propósito es
    //retirarlo de la QList enemies además de emitir la señal update_index,
    //para que los enemigos restantes actualicen el valor de la variable
    //que almacena su índice dentro de la QList enemies, en caso de
    //ser necesario.

    enemies.removeAt(list_index);
    emit update_index(list_index);
}

void Level::defrost() {

    //Este slot llama al método set_enemies_freez() evaluado en true para
    //descongelar a los enemgios.

    set_enemies_freez(false);
}

void Level::finish_delay() {

    //Este slot es llamado para reanudar el procesamiento de instrucciones del nivel,
    //o en caso de que se haya perdido el nivel (gameover es ture), llame al método
    //change_opacity() del objeto black_screen para finalizar el nivel.

    if (gameover) black_screen->change_opacity(true);
    else instructions_timer->start(500);
}

void Level::finish_level() {

    //Este slot es utilizado para emitir la señal update_level_progress, que le indica a la
    //clase Game que el nivel ha concluido, cuando se envía con los argumentos 0 o 3.

    emit update_level_progress((gameover?0:3));
}

void Level::no_health() {

    //Este slot es activado por la señal no_health de la clase Base, y su propósito es
    //congerla el juego e informarle al jugador de que ha sido derrotado pues la base
    //se ha quedado sin salud.

    gameover = true;
    set_global_freez(true);
    information->display_message(389, 194, "La base ha sido destruida");

    //Como se puede observar en la implementación del slot finish_delay() activado por la señal
    //timeout de delay_timer, si gameover está en true, como en este caso, no se prosigue con
    //el procesamiento de instrucciones del nivel sino que se comienza el proceso para
    //finalizarlo e informar a la clase Game que el jugador ha sido derrotado.

    delay_timer->start(3000);
}

void Level::keyPressEvent(QKeyEvent *event) {

    //Esta método se encarga de procesar las teclas presionadas por el jugador.

    if (event->isAutoRepeat()) return;

    //Para manejar el movimiento de los jugadores usamos el arreglo move_dir, el cual indica si una
    //de las teclas de movimiento está siendo presionada; la correspondencia entre los índices del
    //arrgelo y las teclas es la siguiente:

    //0: Tecla arriba.
    //1: Tecla izquierda.
    //2: Tecla abajo.
    //3: Tecla derecha.

    //En el caso del segundo jugador la correspondencia es:

    //0: W.
    //1: A.
    //2: S.
    //3: D.

    //Para más información sobre el movimiento de los jugadores consultar la documentación del
    //slotr move() de la clase Player.

    else if (event->key() == Qt::Key_Up) player1->move_dir[0] = true;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = true;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = true;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = true;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = true;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = true;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = true;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = true;

    //En caso de que estemos mostrando un diálogo en la pantalla, es decir, dialoguing es true,
    //llamamos el método next_dialog() para continuar con el siguiente diálogo.

    //NOTA: Qt::Key_Return corresponde a la tecla Enter, la que está más cerca a las flechas del teclado,
    //Qt::Key_Enter corresponde al Enter del Numeric Keypad.

    else if (dialoguing and (event->key() == Qt::Key_Return)) next_dialog();

    //En caso de que nos encontremos en el menú de pausa, es decir, pause es true,
    //si el jugador presiona la tecla P debemos enviarlo de nuevo al menú de niveles.

    else if ((event->key() == Qt::Key_P) and pause) emit update_level_progress(1);

    //Durante un nivel, el jugador puede acceder al menú de pausa presionando la tecla Esc, sin embargo,
    //esto no se lo podemos permitir siempre pues hay momentos donde se llevan a cabo procesos que no
    //deben ser interrumpidos, como guardar la partida por ejemplo, por lo cual cuando se presiona
    //Esc comprobamos tres condiciones más para decidir que hacer.

    //Si estamos mostrando información en la pantalla, no podemos dejar que se entre en el menú de pausa,
    //pues este no permitiría ver la información, por lo cual si el objeto information está colocado en
    //la escena, no entramos al menú, a excepción de cuando ya se está dentro del menú y se desea
    //cerrarlo de nuevo, en cuyo caso el valor de pause sería true y se cumpliría el condicional.

    //Durante el tutorial no permitimos al jugador acceder a la pausa en ningún momento porque desde esta
    //se puede acceder al menú de niveles, lo cual es un problema si el jugador aún no ha superado el
    //tutorial.

    else if ((event->key() == Qt::Key_Escape) and (pause or (information->scene() == nullptr)) and !tutorial_level) {

        //Si entramos en este condicional es porque necesitamos colocar el menú o retirarlo, por lo cual
        //si estamos en el menú (pause es true), lo retiramos; o en caso contrario lo colocamos.

        if (pause) removeItem(information);
        else information->display_message(389, 182, "\t\t     PAUSA\n\n"
                                                    "Presiona Esc de nuevo para volver al juego.\n"
                                                    "Presiona la tecla P para volver al menú\n"
                                                    "de niveles.");

        //El método set_global_freez() suspende el movimiento de todos los objetos de la escena si su argumento
        //es true, o les devuelve el movimiento en caso de que se evalúe en false. Notemos que si pause es false,
        //set_global_freez() suspende el movimiento pues se evalúa en true, esto es lógico ya que se está
        //entrando en el menú de puasa, mientras que si es true, el movimiento es devuelto puesto que se
        //está saliendo de este.

        set_global_freez(!pause);
        pause = !pause;
    }

    //Un detalle importante es que si el atributo freez de un objeto de la clase Player está en true, no debemos
    //permitirle colocar fire balls, rocas o pegamentos, por lo cual en caso de que el atributo freez del objeto
    //se encuentre en true, retornamos de esta método. Es importante que este posible retorno se coloque luego
    //de los condicionales para modificar el arreglo move_dir del objeto player1 o player2, esto es para que
    //en el caso de que los jugadores mantenga teclas presionadas y retiren la pausa, el juego reaccione
    //correctamente.

    //Notemos que basta preguntar sólo al primer jugador pues él está congelado si y solo si el segundo también.

    else if (player1->freez) return;

    //En caso de que los jugadores no estén congelados (su atributo freez esté en false) y presionen la tecla
    //para atacar (Enter, o Espacio en caso del segundo jugador), llamamos el método add_fire_ball().

    //Recordemos que la tecla para el Enter cercano a las flechas corresponde a Qt::Key_Return,
    //Qt::Key_Enter es utilizado para el Enter del Numeric Keypad.

    else if ((event->key() == Qt::Key_Return)) add_fire_ball(player1->x(), player1->y());
    else if (two_players and (event->key() == Qt::Key_Space)) add_fire_ball(player2->x(), player2->y());

    //Por último, para indicarle al jugador donde puede colocar las rocas, cuando presiona la tecla Borrar,
    //la que está arriba del Enter (o la tecla V en caso del segundo jugador), colocamos una plantilla
    //sobre el mapa que indica los logares donde se pueden colocarlas rocas; esta plantilla también
    //puede ser colocada por el segundo jugador al presionar la tecla V, por lo cual utilizamos
    //la variable entera template_on para saber si al menos uno de los dos está colocando la
    //plantilla; si template_on es mayor que cero es porque sí, por lo cual no hace falta
    //colocarla de nuevo, mientras que si template_on es 0, es porque ninguno de los dos
    //jugadores está colocando la plantilla, por lo cual la agregamos a la escena y
    //aumentamos la variable template_on (el valor de esta variable es disminuido
    //dentor de la reimplementación del método keyReleaseEvent() para
    //esta clase).

    else if ((event->key() == Qt::Key_Backspace) or (two_players and (event->key() == Qt::Key_V))) {
        if (template_on == 0) addItem(power_template);
        template_on++;
    }
}

void Level::keyReleaseEvent(QKeyEvent *event) {

    //Este método se encarga de procesar las teclas que el jugador deja de presionar.

    if (event->isAutoRepeat()) return;

    //De forma análoga a la reimplementación de keyPressEvent(), si el jugador deja de presionar alguna de las
    //teclas de movimiento, lo notificamos dentro del arreglo move_dir de los objetos de clase Player.

    if (event->key() == Qt::Key_Up) player1->move_dir[0] = false;
    else if (event->key() == Qt::Key_Left) player1->move_dir[1] = false;
    else if (event->key() == Qt::Key_Down) player1->move_dir[2] = false;
    else if (event->key() == Qt::Key_Right) player1->move_dir[3] = false;

    else if (two_players and (event->key() == Qt::Key_W)) player2->move_dir[0] = false;
    else if (two_players and (event->key() == Qt::Key_A)) player2->move_dir[1] = false;
    else if (two_players and (event->key() == Qt::Key_S)) player2->move_dir[2] = false;
    else if (two_players and (event->key() == Qt::Key_D)) player2->move_dir[3] = false;

    //El propósito del siguiente condicional es análogo al que se encuentra en keyPressEvent().

    else if (player1->freez) return;

    //Como se mencionó en la reimplementación el método keyPressEvent(), cuando se presiona la tecla Borrar
    //(o V en caso del segundo jugador), colocamos una plantilla en el mapa para indicar los lugares donde
    //se pueden colocar rocas, mientras que la forma de colocarlas es soltando la tecla correspondiente.

    //Recordemos que si la variable template_on es mayor que cero, significa que la plantilla está siendo
    //colocada por al menos uno de los jugadores, por lo cual si luego de restarle uno continua siendo
    //mayor a cero, es porque el otro jugador aún sigue presionando su tecla para colocar rocas, por
    //lo cual no removemos la plantilla, o si la removemos en caso contrario; en cualquier
    //situación siempre llamamos el método add_rock(), el cual ya se encarga de validar
    //si es posible colocar la roca en esa casilla.

    //NOTA: Si realizamos la divisón entera de la posición de los jugadores, obtendremos el índice de
    //la casilla donde se encuentran.

    else if (event->key() == Qt::Key_Backspace) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player1->y()/60, player1->x()/60);
    }
    else if (two_players and (event->key() == Qt::Key_V)) {
        template_on--;
        if (template_on == 0) removeItem(power_template);
        add_rock(player2->y()/60, player2->x()/60);
    }

    //Como no hay que colocar plantilla a la hora de agregar pegamentos pues estos se pueden colocar por
    //todo el mapa, a excepción de la base, simplemente llamamos el método add_glue(), pues ya este se
    //encarga de verifica si no hay una roca o no se desea colocar el pegamento sobre la base.

    else if (event->key() == Qt::Key_Shift) add_glue(player1->y()/60, player1->x()/60);
    else if (two_players and (event->key() == Qt::Key_B)) add_glue(player2->y()/60, player2->x()/60);
}

void Level::add_fire_ball(short x, short y) {

    //Este método agrega a la escena un objeto de la clase FireBall.

    //Si se consulta la documentación de la clase FireBall, se verá que ella misma se encarga de liberar
    //la memoria que aquí se está reservando.

    fire_ball = new FireBall(x, y);
    addItem(fire_ball);

    //El método test_collisions() de la clase FireBall se encarga de interactuar con el resto de objetos de la
    //escena, sin embargo lo tenemos que llamar manualmente, pues no lo podemos hacer dentro del constructor
    //puesto que primero lo debemos agregar a la escena, por lo cual debe ser un método público.

    fire_ball->test_collisions();
}

bool Level::border(short i, short j) {

    //Este método recibe los índices de una casilla y retorna true si corresponden a una casilla donde se
    //puede colocar una roca, o false en caso contrario. Para ver en cuales casillas se pueden colocar
    //rocas consultar la documentación del método set_targets() de la clase Enemy.

    if (((i == 2) or (i == 6)) and (3 < j) and (j < 9)) return true;
    else if (((j == 3) or (j == 9)) and (2 < i) and (i < 6)) return true;
    else return false;
}

void Level::add_rock(short i, short j) {

    //Este método se encarga de verificar que en la casilla ingresada se pueda colocar una roca,
    //y únicamente en caso afirmativo la coloca.

    //Primero verificamos que la cantidad de rocas que posee el jugador no sea nula, luego que
    //la casilla donde desea colocar una roca sea en principio válida, posteriormente verificamos
    //que en esa casilla no haya nada, o en caso de que sí haya un objeto del terreno, este no
    //sea una roca (terrain->tiles[i][j]->get_type() != 1).

    if (border(i, j)  and ((*rocks_num) != 0) and
       ((terrain->tiles[i][j] == nullptr) or (terrain->tiles[i][j]->get_type() != 1))) {

        //Si todas las condiciones anteriores se han cumplido, la casilla es válida en lo que respecta a los
        //objetos del mapa, pero hay una última condicion que se debe satisfacer, que no haya un enemigo en
        //la casilla objetivo; para este propósito utilizamos el objeto ghost_rock, el cual es un
        //rectángulo que colocamos en la casilla objetivo y revisamos con qué objetos de la
        //escena está colisonando, en caso de que colisione con algún enemigo no podemos
        //permitirle al jugador colocar la roca.

        //Notése el hecho de que no hace falta agregar el objeto a la escena para que el método collidingItems(),
        //retorne correctamente la QList con los objetos de la escena con los que éste está colisionando.

        QGraphicsItem *item;

        //Colocamos el objeto en la posición de la casilla objetivo.

        ghost_rock->setPos(60*j, 60*i);
        rock_ghost_collisions = collidingItems(ghost_rock, Qt::IntersectsItemBoundingRect);

        //Analizamos la clase de los objetos con los que colisiona el objeto ghost_rock.

        for (short i = 0; i < rock_ghost_collisions.size(); i++) {
            item = rock_ghost_collisions[i];

            //No hace comparar contra todas las clases de enemigos, en realidad estos cuatro son los únicos que
            //podrían llegar a entrometerse.

            if ((typeid(*item) == typeid(Enemy)) or (typeid(*item) == typeid(Snail))
             or (typeid(*item) == typeid(Porcupine)) or (typeid(*item) == typeid(Chamaleon))) {

                //En caso de que el objeto ghost_rock colisione con alguno de ellos retornamos pues
                //no podemos colocar la roca.

                return;
            }
        }

        //En caso de que el flujo del método llegue hasta aquí es porque ahora sí se ha cumplido en su totalidad
        //las condiciones para colocar una roca, por lo cual procedemos con esta tarea.

        if (terrain->tiles[i][j] != nullptr) {

            //Si hay algo en la casilla objetivo (no puede ser una roca porque no se hubiera satisfecho la
            //condición del primer condicional), lo removemos de la escen y eliminamos del objeto terrain.

            //NOTA: Siempre que se libera la memoria de uno de los TerrainObjetos del objeto terrain debemos
            //dejar el puntero apuntando hacia nullptr, pero en este caso no hace falta pues agregaremos
            //la roca.

            removeItem(terrain->tiles[i][j]);
            delete terrain->tiles[i][j];
        }

        //Creamos el TerrainObject correspondiente a una roca y lo agregamos
        //a la matriz tiles del objeto terrain.

        terrain->tiles[i][j] = new TerrainObject(i, j, 1);
        addItem(terrain->tiles[i][j]);

        //Removemos de la escena una de las imágenes que indican la cantidad de
        //rocas que posee el jugador, además de reducir el valor de *rocks_num.

        removeItem(rock_powers + (*rocks_num) - 1);
        (*rocks_num)--;

        //Por último, aumentamos el contador rocks_num del objeto terrain puesto
        //que ahora posee una roca más.

        terrain->rocks_num++;
    }
}

void Level::add_glue(short i, short j) {

    //Este método se encarga de validar que en la casilla ingresada se pueda colocar una carga
    //de pegamenteo, y en caso afirmativo, la coloca.

    //Si el jugador no posee cargas de pegamento o la desea colocar sobre la base, retornamos.

    if (((*glues_num) == 0) or ((i == 4) and (j == 6))) return;
    else if (terrain->tiles[i][j] == nullptr) {

        //En caso de que en la casilla objetivo no haya ningún objeto, simplemente seguimos un
        //proceso análogo al de agregar una roca, pero para una carga de pegamento.

        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(glue_powers + (*glues_num) - 1);
        (*glues_num)--;
    }
    else if (terrain->tiles[i][j]->get_type() == 2) {

        //En caso de que en la casilla objetivo haya un pegamento de caracol, lo removemos
        //de la escena y colocamos la carga de pegamento en su lugar.

        removeItem(terrain->tiles[i][j]);
        delete terrain->tiles[i][j];

        terrain->tiles[i][j] = new TerrainObject(i, j, 3);
        addItem(terrain->tiles[i][j]);

        removeItem(glue_powers + (*glues_num) - 1);
        (*glues_num)--;
    }

    //En caso de que en la casilla objetivo haya una roca, o un pegamento colocado por los jugadores
    //previamente, no hacemos nada.
}

void Level::set_global_freez(bool freez) {

    //Este método se encarga de congelar todos los objetos de la escena si freez es true,
    //o descongelarlos en caso contrario.

    //Primero llamamos el método set_enemies_freez() para congelar o descongelar a todos
    //los enemigos de la pantalla.

    set_enemies_freez(freez);

    //Luego, en caso de que haya un power up (power_up no es nullptr), también lo
    //congelamos mediante el método set_freez() de la clase power_up.

    if (power_up != nullptr) power_up->set_freez(freez);

    //Congelamos al jugador, o a los jugadores.

    player1->freez = freez;
    if (two_players) player2->freez = freez;

    //Puesto que en un momento cualquiera, en el nivel hay distintos timers que pueden estar corriendo o
    //detenidos, si queremos detener el juego por un momento y luego retomarlo justo como estaba,
    //debemos detener estos timers, además de guardar el tiempo que les faltaba, para esto
    //utilizamos los arreglos active_timers y remainings, donde active_timers indica
    //cuales timers estaban activos al momento del congelamiento, mientras que
    //remainings indica el tiempo que les restaba.

    //La correspondiencia entre los índices y los timers es la siguiente:

    //0: instruction_timer.
    //1: delay_timer.
    //2: freez_timer.

    //En donde este último timer es utilizado cuando se otorga el power up que congela
    //a los enemigos, para más información consultar la documentación del método
    //give_power().

    if (freez) {

        //Si vamos a congelar el juego, actualizamos la información de los arreglos active_timers
        //y remainings, y a la par vamos deteniendo los timers.

        if (instructions_timer->isActive()) {
            active_timers[0] = true;
            remainings[0] = instructions_timer->remainingTime();
            instructions_timer->stop();
        }
        else active_timers[0] = false;

        if (delay_timer->isActive()) {
            active_timers[1] = true;
            remainings[1] = delay_timer->remainingTime();
            delay_timer->stop();
        }
        else active_timers[1] = false;

        if (freez_timer->isActive()) {
            active_timers[2] = true;
            remainings[2] = freez_timer->remainingTime();
            freez_timer->stop();
        }
        else active_timers[2] = false;
    }
    else {

        //En caso de que vayamos a descongelar el juego, recuperamos la información de los timers
        //y pondemos en marcha los indicados por el arreglo active_timers.

        if (active_timers[0]) {
            instructions_timer->start(remainings[0]);
            active_timers[0] = true;
        }
        if (active_timers[1]) {
            delay_timer->start(remainings[1]);
            active_timers[1] = true;
        }
        if (active_timers[2]) {
            freez_timer->start(remainings[2]);
            active_timers[2] = true;
        }
    }

    //Notése que no hace falta llevar el registro de action_timer, pues este solo se usa en el tutorial,
    //en el cual no le permitimos al jugador entrar al menú de pausa por las razones expuestas dentro
    //de la documentación de la reimplementación del método keyPressEvent() para esta clase.
}
