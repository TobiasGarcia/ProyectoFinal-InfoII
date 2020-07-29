#include "enemy.h"

Enemy::Enemy(short i, short j, short _type, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
             type(_type), terrain(_terrain), level(_level), list_index(_list_index) {

    //Utilizamos el método define_personality() para definir la caracteristicas propias del enemigo que dependen
    //del valor de type, para más información dirigirse al método define_personality().

    define_personality();

    setPos(tiles2pixels(i, j).toPoint());
    setPixmap(*pix);

    //Le colocamos el Z value en 2, pues los enemigos pertenecen a la segunda capa.

    //Las capas son las siguientes:

    //0 ---La plantilla para colocar las rocas.
    //1:-----Los obejtos del terreno y la base.
    //2:--------------------------Los enemigos.
    //3:-------------------------Los power ups.
    //4:------------------------Las fire balls.
    //5:----------------------El hud del juego.
    //6:-------------------------Los jugadores.
    //7:------------------La clase Information.
    //8:------------------La clase BlackScreen.

    setZValue(2);

    //La variable freez representará cuando el enemigo se puede mover y cuando no, false para moverse y true
    //para permanecer estático; la variable defeated y health_bar_on son explicadas dentro del método
    //reduces_health(); la variable heatlh lleva la cuenta de cuanta vida le queda al enemigo.

    freez = false;
    defeated = false;
    health = max_health;
    health_bar_on = false;

    initialize_health_bar();

    //Colocamos los targets del enemgio y llamamos el método update_target() para indicarle
    //que tiene un nuevo target.

    set_targets(i, j);
    update_target();

    //Inicalizamos los timers y comenzamos a mover al enemigo llamando al método start()
    //de move_timer.

    initialize_timers();
    move_timer->start(50);
}

void Enemy::initialize_timers() {

    //Inicializamos los timers.

    //El timer bite_timer es activado cuando el enemigo choca con la base, y su propósito es
    //comenzar a bajarle salud a esta periódicamente.

    bite_timer = new QTimer;

    //Este timer es utilizado para que cuando el enemigo es atacado por los jugadores, su barra de salud
    //aparezca por encima de este durante un tiempo.

    health_on_timer = new QTimer;
    health_on_timer->setSingleShot(true);
    connect(health_on_timer, &QTimer::timeout, this, &Enemy::health_off);

    //Este timer activa el slot move, el cual tiene como propósito modificar la posición del
    //enemigo para darle movimiento.

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Enemy::move);

    //Cuando un enemigo choca contra una roca, actualizamos sus targets para que siga otro camino,
    //sin embargo, por gusto personal, este timer se utiliza para que además de recalcular los
    //targets, haya un pequeño delay antes de que el enemigo comience a moverse de nuevo.

    delay_timer = new QTimer;
    delay_timer->setSingleShot(true);
    connect(delay_timer, &QTimer::timeout, this, &Enemy::finish_delay);
}

void Enemy::initialize_health_bar() {

    //Este método inicializa la que será la barra de salud del enemigo, la cual está compuesta por dos
    //rectangulos, uno de fondo de 40 x 7 pixeles y otro encima de 38 x 5, este último se va reduciendo
    //cuando el enemigo pierde salud, mientras que el otro permanece con las mismas dimensiones, esto
    //es simplementa para dar un buen aspecto visual.

    health_bar = new QGraphicsRectItem[2];
    health_bar->setBrush(QColor(86, 86, 86));
    health_bar->setPen(QColor(66, 66, 66));
    health_bar->setRect(0, 0, 40, 7);
    health_bar->setZValue(2);

    (health_bar + 1)->setParentItem(health_bar);
    (health_bar + 1)->setBrush(QColor(54, 104, 195));
    (health_bar + 1)->setPen(QColor(54, 104, 195));
    (health_bar + 1)->setRect(1, 1, 38, 5);
    (health_bar + 1)->setZValue(2);
}

void Enemy::define_personality() {

    //Definimos las caracteristicas que definene al enemigo según su tipo.

    if (type == 0) {
        //36 x 36 pixeles;
        width_half = 18;
        height_half = 18;
        spd = 30;
        max_health = 500;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy0.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 1) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 25;
        max_health = 800;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy1.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 2) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 40;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/enemy2.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 3) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 35;
        max_health = 600;
        pix = new QPixmap(":/enemies/resources/images/enemies/snail.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 4) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 40;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/porcupine.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 5) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 35;
        max_health = 500;
        pix = new QPixmap(":/enemies/resources/images/enemies/owl.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 6) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 25;
        max_health = 700;
        pix = new QPixmap(":/enemies/resources/images/enemies/chamaleon.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else if (type == 7) {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 25;
        max_health = 300;
        pix = new QPixmap(":/enemies/resources/images/enemies/mole.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
    else {
        //40 x 50 pixeles;
        width_half = 20;
        height_half = 25;
        spd = 140;
        max_health = 500;
        pix = new QPixmap(":/enemies/resources/images/enemies/vulture.png");
        *pix = pix->transformed(QTransform().scale(0.5, 0.5));
    }
}

Enemy::~Enemy() {
    delete pix;
    delete delay_timer;
    delete move_timer;
    delete bite_timer;
    delete health_on_timer;
    delete[] health_bar;
}

void Enemy::reduces_health(short hit) {

    //Este método se encarga de reducirle la salud al enemigo cuando este es atacado, además de colocar
    //su barra de vida para que se puede ver cuanta salud le resta.

    //Primero restamos el valor de hit a la salud actual del enemigo, en caso de que esta resta resulte en un
    //valor negativo, colocamos la salud en 0.

    health -= hit;
    if (health < 0) health = 0;

    //Actualizamos la longitud de la barra de vida del enemigo según su vida restante.

    (health_bar + 1)->setRect(1, 1, 38*(health/float(max_health)), 5);

    //Por gusto personal, programé para que cuando el enemigo es atacado, su barra de salud se coloque sobre él
    //por un cierto período de tiempo, además de que si el enemigo vuelve a ser atacado dentro de este mismo
    //período, el tiempo de duración de la barra de salud se reinicia.

    //La variable health_bar_on indica cuando el enemigo tiene su barra de vida sobre él, de esta forma sabemos
    //cuando debemos colocarla, esto es para evitar que se añada varias veces a la escena, lo cual es un error.

    if (!health_bar_on) {
        health_bar_on = true;
        health_bar->setPos(x() - 20, y() - 35);
        level->addItem(health_bar);
    }

    //El timer health_on_timer activa el slot health_off, el cual se encarga de retirar la barra de salud de la
    //escena. Notemos que como se mencionó antes, siempre que el enemigo sea atacado el tiempo de health_on_timer
    //es reiniciado.

    health_on_timer->start(700);

    if (health == 0) {

        //Por último, si la salud del enemigo ha llegado a 0, emitimos la señal remove_enemy() con el índice de la
        //posición dentro de la QList enemies de la clase Level (ver documentación de la clase Level),
        //correspondiente al enemigo que lanza la señal.

        emit remove_enemy(list_index);

        //El siguiente condicional es para, el muy particular caso, en que el jugador toma el power up para
        //atacar a todos los enemigos justo cuando este enemigo especificamente se agregó a la QList enemies
        //de Level, pero aún no se ha agregado a la escena y este perdió toda su salud por el power up.

        if (scene() != NULL) level->removeItem(this);

        //Detenemos el timer que reduce la salud de la base (ver documentación del método collisions_handler())
        //en caso de que éste se encuentre activo.

        if (bite_timer->isActive()) bite_timer->stop();

        //Notemos que de la escena hemos eliminado al enemigo, pero aún queda la barra de salud, si utilizamos
        //delete en este momento sobre el enemigo como tal, esto haría que la barra de vida también desapareciese
        //junto con él, sin embargo, prefería que esta última durara un poco más luego de retirar al enemigo,
        //esto es pues cuando se borraba la barra de salud junto con el enemigo, para el enemigo en
        //particular de la clase Chamelon, el cual se hace invisible, podía resultar confuso saber
        //si había sido derrotado o se había vuelto invisible de nuevo, y como me gustó ese
        //detalle cuando lo agregué, preferí colocárselo al resto de enemigos también.

        //En este orden de ideas, la variable defeated le indica al slot health_off() que además de reitrar la
        //barra de salud del enemigo, también lo elimine. La sentencia freez = true es necesaria pues ocurre
        //que cuando el enemigo es retirado de la escena, como aún no se ha eliminado, la barra de vida se
        //sigue moviendo, por lo cual mediante la variable freez lo detenemos.

        defeated = true;
        freez = true;
    }
}

void Enemy::health_off() {

    //Colocamos health_bar_on en false y removemos la barra de salude del enemigo de la escena; en caso de que
    //la varible defeated se encuentre en true, debemos eliminar al enemigo pues este ha sido derrotado.

    health_bar_on = false;
    level->removeItem(health_bar);
    if (defeated) delete this;
}

void Enemy::set_targets(short i, short j) {

    //Este método calcula los targets que debe seguir el enemigo para llegar hasta la base del jugador,
    //dependiendo de la casilla donde aparece y la distribución de rocas del terreno en ese momento.

    //Recordemos que la escena del nivel está dividida en una matriz de 9 x 13 casillas, y sólo en unas en
    //particular los jugadores pueden colocar las rocas, la siguiente figura se utiliza para ilustrar esas
    //casillas además de otras caracteristicas:

    //    -1  0  1  2  3  4  5  6  7  8  9 10 11 12 13
    // -1     X  X  X  X  X  X  X  X  X  X  X  X  X
    //  0  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  1  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  2  X  0  0  0  0  1  1  1  1  1  0  0  0  0  X
    //  3  X  0  0  0  1  0  0  0  0  0  1  0  0  0  X
    //  4  X  0  0  0  1  0  0  B  0  0  1  0  0  0  X
    //  5  X  0  0  0  1  0  0  0  0  0  1  0  0  0  X
    //  6  X  0  0  0  0  1  1  1  1  1  0  0  0  0  X
    //  7  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  8  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  9     X  X  X  X  X  X  X  X  X  X  X  X  X

    //Donde los 1 son las casillas donde se pueden colocar rocas y B es la casilla donde se encuentra la base.

    //Los enemigos aparecen afuera de la pantalla, en las casillas marcadas con X. El proceso para calcular los
    //targets del enemigo se puede dividir en tre etapas: sondeo, pasos intermedios y entrada. Durante el sondeo
    //se consultan las posiciónes donde se pueden colocar rocas, hasta encontrar una entrada, el sondeo se realiza
    //por lados, por ejemplo, para un enemigo puede que primero se consulten todas las casillas donde pueden ir
    //rocas que se encuentran a la izquierda de la base, en segundo lugar las que están por debajo, luego las
    //que están por encima, y por último las que están a la derecha; este orden de consulta depende de la
    //posición inicial del enemigo, de tal forma que se van buscando las posibles entrads más proximas a
    //este y se van dejando para el final las más lejanas.

    //Una pregunta que puede surgir es: ¿qué es lo que sucede cuando no hay entradas disponibles?, este escenaro
    //es controlado mediante el número de rocas que se les va otorgando al usuario y las que están puestas sobre
    //el terreno, siempre controlando que el número total de rocas nunca supere el valor de 15, de esta forma no
    //es posible que no exista por lo menos una entrada disponible.

    //Luego de identificar una entrada, se pasa al proceso de los pasos intermedios, el cual consiste simplemente
    //en agregar los targets que requiera el enemigo para llegar hasta la hilera exterior más cercana a la
    //entrada.

    //Y finalmente, se llega a la etapa de entrada, en donde se agregan 3 targets finales para que el enemigo
    //pueda pasar la frontera de rocas.

    //Otra preguntar que puede surgir en este momento es: si los targets son calculados según la distribución de
    //rocas que hay cuando aparece el enemigo, ¿qué sucede si mientras este se desplaza hacia la base se bloqueda
    //su camino mediante una roca?, exiten dos escenarios, el primero es cuando el enemigo está muy cerca del
    //lugar donde se colocará la roca, en este escenario simplemente no le permitimos al usuario colocar la roca
    //para que no le caiga encima al enemigo, por otro lado, si este no se encuentra cerca de donde se quiere
    //colocar la roca, permitimos que el usuario la coloque y dentro del slot move(), cuando detectamos la
    //colisión con la roca, simplemente devolvemos un poco al enemigo hacia atrás y recalculamos los targets.

    //La variable gap es utilizada para facilitar el proceso de entrada; el arreglo probe, almacenará el orden en
    //que se sondearán las hileras, los valores que almacena y las direcciones tiene la siguiente correspondencia:

    //0: Hilera superior.
    //1: Hilera izquierda.
    //2: Hilera inferior.
    //3: Hilera derecha.

    short gap[4] = {1, 0, -1, 0}, tile[2], probe[4];

    //A continuación llenamos los valores de probe según la posición inicial el enemigo, recordemos que como estos
    //aparecen fuera de la pantalla, las variables i y j pueden tomar valores desde -1 hasta 9 para i, y desde -1
    //hasta 13 para j.

    if ((i == -1) or (i == 9)) {
        probe[0] = (i + 1)/5;
        probe[1] = (j<=6)?1:3;
    }
    else {
        probe[0] = (j + 8)/7;
        probe[1] = (i<4)?0:2;
    }

    probe[2] = (probe[1] + 2)%4;
    probe[3] = (probe[0] + 2)%4;

    //Según donde encontremos una entrada, utilziamos gap para terminar
    //de ajustar la forma en que el enemigo va a entrar.

    //Luego de llenar probe, utilizamos un ciclo para testear cada hilera mediante el método entrance_exists(),
    //el cual, en caso de haber encontrado una entrada, almacena la casilla de esta dentro del arreglo tile;
    //cuando encontramos la entrada, mediante el método middle_steps() calculamos los pasos intermedios para
    //que el enemigo pueda acercarse a la hilera donde está la entrada, el valor de k indica en cual de las
    //direcciones de probe se haya esta última; para finalizar, con la ayuda de gap, agregamos los últimos
    //targets correspondientes al proceso de entrada; recordemos que en esta última situación probe[k]
    //representa la hilera donde se encuentra la entrada. Este proceso puede parecer muy abstracto,
    //pero cuando se le sigue el flujo con calma resulta ser bastante intuitivo.

    for (short k = 0; k < 4; k++) {
        if (entrance_exists(probe[k], tile)) {
            middle_steps(k, probe);
            targets.enqueue(tiles2pixels(tile[0] - gap[probe[k]], tile[1] + gap[(probe[k] + 1)%4]));
            targets.enqueue(tiles2pixels(tile[0] + gap[probe[k]], tile[1] - gap[(probe[k] + 1)%4]));
            targets.enqueue(tiles2pixels(4, 6));
            return;
        }
    }
}

bool Enemy::entrance_exists(short side, short tile[2]) {

    //Retorna true en caso de que en la hilera de rocas correspondiente al lado side (ver correspondencia en
    //la documentación del método set_targets()) haya una entrada para el enemigo, en cuyo caso almacena la
    //casilla de esta dentro de tiles; retorna false en caso contrario y no modifica el arreglo tiles.

    //Puesto que me parecía que programar a los enemigos para que cuando hubieran varias entradas en una misma
    //hilera, escogiesen la más cercada, era algo muy predecible, haciendo que el juego no fuera tan entretenido,
    //por lo cual, en caso de que hayan varias entradas por una misma hilera, escogemos una al azar. Para este
    //propósito utilizamos el vector entrances_index, en el cual almacenamos indices correspondientes a entradas
    //de la misma hilera, para que al final nos quedemos con alguno de ellos de forma aleatoria.

    short index;
    QVector<short> entrances_index;

    //Por una hilera de rocas como máximo pueden haber 5 entradas, por lo cual le indicamos al vector
    //entrances_index que reserve memoria para no más de 5 entradas. Notemos que aunque sabemso que no
    //serán más de 5 entradas, es preferible utilizar un vector a un array, pues como tomaremos un elemento
    //aleatorio de este, en caso de utilizar un array nos tocaría utilizar una variable adicional para llevar
    //la cuenta de cuantos índices hemos agregado, que en el caso del vector, basta simplemente llamar al
    //método size().

    entrances_index.reserve(5);

    //Hay dos posibilidades, que la hilera esté en los costados, o que la hilera sea la de arriba o abajo,
    //estos casos deben recibir un tratamiento diferente pues por ejemplo en las hileras de los costados,
    //el índice variable es la i mientras que j es 3 o 9, por otra parte en las hileras de arriba o abajo,
    //el índice variable es j, mientras que i es 2 o 6; para esto utilizamos el siguiente condicional, pues
    //los lados impares corresponden a los cotados mientras que los pares a arriba o abajo.

    if (side%2) {

        //Como vamos por los costados, el índice fijo es j, cuyo valor 3 o 9, el cual es almacenado
        //en la variable index.

        index = 3*side;
        for (short i = 3; i < 6; i++) {

            //Si en terrain hay un puntero a nulo o hay un objeto diferente de una roca, es porque esa casilla
            //es una entrada váilda.

            //NOTA: Ver documentación de la clase terrain para una explicación del porqué almacena punteros.

            if ((terrain->tiles[i][index] == nullptr)
             or (terrain->tiles[i][index]->get_type() != 1)) entrances_index.push_back(i);
        }

        //Luego del anterior ciclo, si entrances_index continua estando vacío, es porque la hilera en cuestión
        //no contaba con entradas, por lo cual retornamos false, o elegimos una entrada al azar en caso de que
        //si existan, y retornamos true.

        if (entrances_index.isEmpty()) return false;
        else {
            tile[0] = entrances_index[rand()%entrances_index.size()];
            tile[1] = index;
            return true;
        }
    }
    else {

        //Como vamos por arriba o abajo, el índice fijo es i, cuyo valor 2 o 6, el cual es almacenado
        //en la variable index.

        //El proceso es análogo al de las hileras de los costados.

        index = 2*side + 2;
        for (short j = 4; j < 9; j++) {
            if ((terrain->tiles[index][j] == nullptr)
             or (terrain->tiles[index][j]->get_type() != 1)) entrances_index.push_back(j);
        }

        if (entrances_index.isEmpty()) return false;
        else {
            tile[0] = index;
            tile[1] = entrances_index[rand()%entrances_index.size()];
            return true;
        }
    }
}

void Enemy::middle_steps(short k, short probe[4]) {

    //Este método se encarga de agregar los targets intermedios para que el enemigo pueda llagar hasta la hilera
    //exterior más cercana a la entrada, entiéndase por hilera exterior a las que poseen la letra 'E' en la
    //siguiente figura:

    //    -1  0  1  2  3  4  5  6  7  8  9 10 11 12 13
    // -1     X  X  X  X  X  X  X  X  X  X  X  X  X
    //  0  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  1  X  0  0  E  E  E  E  E  E  E  E  E  0  0  X
    //  2  X  0  0  E  0  1  1  1  1  1  0  E  0  0  X
    //  3  X  0  0  E  1  0  0  0  0  0  1  E  0  0  X
    //  4  X  0  0  E  1  0  0  B  0  0  1  E  0  0  X
    //  5  X  0  0  E  1  0  0  0  0  0  1  E  0  0  X
    //  6  X  0  0  E  0  1  1  1  1  1  0  E  0  0  X
    //  7  X  0  0  E  E  E  E  E  E  E  E  E  0  0  X
    //  8  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  9     X  X  X  X  X  X  X  X  X  X  X  X  X

    //Recordemos que k representa en cual de las direcciones de probe se ha encontrado la entrada,
    //ejemplos, si k es 2, es porque la entrada se ha encontrado en la tercera dirección que se
    //sondeó.

    //Si k es 0, es porque la entrada está justo en la hilera del frente del enemigo, por lo cual
    //no hay que ayudarle para llegar hasta la hilera exterior más cercana.

    if (k == 0) return;

    //Si k es 1 o 2, es porque la hilera donde se encontró la entrada no es la que está al frente del enemigo ni
    //tampoco la que está al lado opuesto del mapa, por lo cual basta con agregar un único paso intermedio.

    else if (k == 1) targets.enqueue(tiles2pixels(sides2point(probe[0], probe[1])));
    else if (k == 2) targets.enqueue(tiles2pixels(sides2point(probe[0], probe[2])));

    //Y en caso de que k sea 3, es porque la entrada fue encontrada en la hilera del lado opuesto del mapa,
    //por lo cual hace falta agregar dos pasos intermedios para llegar hasta la hilera exterior más cercana.

    else {
        targets.enqueue(tiles2pixels(sides2point(probe[0], probe[1])));
        targets.enqueue(tiles2pixels(sides2point(probe[1], probe[3])));
    }
}

QPoint Enemy::sides2point(short side1, short side2) {

    //Este método recibe dos lados side1 y side2, y retorna alguna de las casillas marcadas con una Z en la
    //siguiente figura, bajo un criterio que se explicará a continuación:

    //    -1  0  1  2  3  4  5  6  7  8  9 10 11 12 13
    // -1     X  X  X  X  X  X  X  X  X  X  X  X  X
    //  0  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  1  X  0  0  Z  E  E  E  E  E  E  E  Z  0  0  X
    //  2  X  0  0  E  0  1  1  1  1  1  0  E  0  0  X
    //  3  X  0  0  E  1  0  0  0  0  0  1  E  0  0  X
    //  4  X  0  0  E  1  0  0  B  0  0  1  E  0  0  X
    //  5  X  0  0  E  1  0  0  0  0  0  1  E  0  0  X
    //  6  X  0  0  E  0  1  1  1  1  1  0  E  0  0  X
    //  7  X  0  0  Z  E  E  E  E  E  E  E  Z  0  0  X
    //  8  X  0  0  0  0  0  0  0  0  0  0  0  0  0  X
    //  9     X  X  X  X  X  X  X  X  X  X  X  X  X

    //NOTA: Recordemos que la definición de hilera exterior esta dada en la documentación del método
    //middle_steps().

    //El criterio utilizado para elegir cual de las casillas marcadas con Z es retornada en función del conjunto
    //{side1, side2}, consiste en la intersección de las hileras exteriores de las direcciones side1 y side2,
    //por ejemplo, si side1 es 0 (arriba) y side2 es 3 (derecha), la casilla retornada es [1, 10], si side1
    //es 3 (derecha) y side2 es 2 (abajo), la casilla retornada es [7, 10]. Notemos que existen combinaciones de
    //lados que no poseen intersección, como la combinación {2, 0} o {1, 3}, no obstante, si se ha comprendido
    //el flujo del programa, no es difícil notar que este tipo de combinaciones no se pueden presentar.

    //El problema ahora radica en cómo asociar todas estas posibles parejas a sus respectivos resultados, y la
    //solución resulta ser muy sencilla, sumamos uno a side1 y side2, cálculamos su producto y comparamos el
    //resultado con 4 valores posibles, el 2, el 4, el 6 y el 12, aunque en realidad no hace falta comparar con
    //el 12 por que el resultado de la operación sólo puede ser alguno de estos números.

    short product = (side1 + 1)*(side2 + 1);

    if (product == 2) return QPoint(1, 2);
    else if (product == 4) return QPoint(1, 10);
    else if (product == 6) return QPoint(7, 2);
    else return QPoint(7, 10);
}

void Enemy::update_target() {

    //Este método modifica la dirección en que se mueve el enemigo, para colocarla en dirección del target del
    //frente de la queue targets, además de rotar la imágen del enemigo para colocarla en esa dirección.

    //Calculamos la dirección que se debe seguir para llegar hasta el target, posteriormente normalizamos ese
    //vector y multiplicamos por la rapidez (spd) predefinida del enemigo, para obtener su vector velocidad.

    dir = targets.head() - QVector2D(pos());
    speed = spd*dir.normalized();

    //Debemos mantener una copia del vector velocidad del enemigo, para que cuando este se vea sometido a un
    //cambio en su rapdidez, como cuando entra en contacto con el pegamento colocado por los jugadores o el
    //que es colocado por el caracol, tengamos el valor original almacenado, para que cuando el efecto
    //termine le podamos volver a colocar el mismo vector velocidad.

    speed_aux = speed;

    //Rotamos la imágen del enemigo para que apunte en la dirección en que se mueven.

    setRotation(90 - (atan2(-dir[1], dir[0])*180/M_PI));
}

bool Enemy::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Este método es utilizado para procesar las colisiones del enemigo. Retorna true en caso de que el enemigo
    //se deba detener súbitamente, o false en caso de que pueda continuar con su camino.

    QGraphicsItem *item;
    for (short i = 0; i < collisions.size(); i++) {

        //Almacenamos el QGraphicsItem con el que se colisionó, y mediante la función typeid(), comparamos para
        //averiguar si este corresponde a una clase que interactúe con el enemigo.

        item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {

            //En caso de ser un objeto de clase TerrainObject, utilizamos un dynamic_cast<>() para bajar en la
            //jerarquía de clases, desde QGraphicsItem hasta TerrainObject.

            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);

            //En caso de que el objeto con que se colisionó sea una roca, es decir, terrain_object->get_type()
            //retorna 1 (ver documentación de la clase TerrainObject), y además el enemigo se encuentra fuera
            //de un rectángulo de 360 x 240 centrado en la base, llamamos el método rock_collision() y retornamos
            //true pues el enemigo no puede dar un paso más.

            //Lo del rectángulo es debido a que cuando los enemigo atravesaban, por una entrada claro, la región
            //donde se colocan las rocas, en ciertas situaciones la rotación de la imágen que se hacía para poder
            //dirijirse hacia la base, hacía que el enemigo entrara en contacto con una roca y este se quedara
            //atorado, pero al agregar la condición del rectángulo esto ya no sucede.

            if ((terrain_object->get_type() == 1) and !QRect(210, 150, 360, 240).contains(x(), y(), false)) {
                rock_collision();
                return true;
            }

            //En caso de que el TerrainObject sea de tipo 2 (pegamento de caracol) o
            //3 (pegamento de los jugadores), simplemente modificamos la magnitud
            //de su vector velocidad, reduciéndola o aumentándola según sea el caso.

            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
            else if (terrain_object->get_type() == 2) speed = 1.4*speed_aux;
        }
        else if (typeid(*item) == typeid(Base)) {

            //En caso de que el enemigo haya colisionado con la base, lo detenemos y llamamos el método start()
            //del timer bite_timer, para que esté comience a reducir la salud de la base periódicamente.

            move_timer->stop();

            //Como queremos que justo cuando el enemigo haga contacto con la base, se comience a bajar la salud
            //de esta última, emitimos la señal firts_bite(), que simplemente activa el slot bitten() del objeto
            //Base, esto es pues a pesar de que bite_timer() activa el slot bitten(), como es el correcto
            //funcionamiento de los timers, el método start() comienza la cuenta regresiva pero no emite
            //la señal de inmediato.

            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

void Enemy::move() {

    //Este slot es activado por move_timer cada 50 milisegundos, su propósito es modificar la posición del enemigo
    //dentro de la escena según se vector velocidad, para darle movimiento.

    //Como se mencionó en el contructor, que la variable freez se encuenre en true significa que el enemigo debe
    //permanecer estático, por lo cual, no debemos hacer nada dentro de este slot.

    if (freez) return;

    //Cuando el enemigo entra en contacto con un pegamento, su rapidez debe modificarse, esto sucede dentro del
    //método collisions_handler(), por lo cual siempre colocamos el vector velocidad original antes de llamar
    //a este método, de esta forma, si dentro de él no se detecta ninguna colisión con pegamento, el vector
    //velocidad se conservará intacto.

    speed = speed_aux;

    //Recordemos que, como se mencionaba en la documentación del método collisions_handler(), si este retorna
    //true es porque debemos detener al enemigo súbitamente.

    if (!collisions_handler(collidingItems(Qt::IntersectsItemBoundingRect))) {

        //En caso de que el enemigo no se tenga que detener, modificamos su posición, el 0.06 representa
        //el intervalo de tiempo de la discretización del movimiento, como este método es llamda cada 0.05
        //segundos, en principio debería ser 0.05, pero para darle una ventana de tiempo a los procesos que
        //tenga que hacer la computadora, utilizamos 0.06 en su lugar.

        //Si health_bar_on es true, es porque el enemigo tiene su barra de salud sobre él, por lo cual también
        //la trasladamos junto con él.

        setPos(x() + 0.06*speed[0], y() + 0.06*speed[1]);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        //Ahora calculamos el vector de dirección hacia el target del frente de la queue targets, con el propósito
        //de revisar su magnitud, porque esta nos indica qué tan cerca estamos del target, por lo cual programamos
        //para que cuando esa distancia sea menor a 7 pixeles, se considere que el enemigo ha llegado al objetivo,
        //por lo cual eliminamos el target del frente de la queue y llamamos el método update_target() para
        //continuar con el siguiente.

        //Notemos que nunca nos quedaremos sin targets, pues el último siempre es la base, por lo cual el enemigo
        //colisionará contra esta antes de llegar hasta el centro.

        dir = targets.head() - QVector2D(pos());

        if (dir.length() < 7) {
            targets.dequeue();
            update_target();
        }
    }
}

void Enemy::rock_collision() {

    //Este método se encarga de procesar la colisión del enemigo con una roca.

    //Primero devolvemos al enemigo un poco en la dirección en la que se movía; si su barra de salud está colocada
    //sobre él, también la devolvemos.

    setPos(x() - 0.15*speed[0], y() - 0.15*speed[1]);
    if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

    //Limpiamos la queue targets, almacenamos en el arreglo tile la casilla calculada por el método
    //recalculate_initial_tile(), llamamos de nuevo el método set_targets() y realizamos
    //intencionlamente un pequeño delay deteniendo a move_timer y llamando el método
    //start() de delay_timer.

    short tile[2];
    targets.clear();
    recalculate_initial_tile(tile);
    set_targets(tile[0], tile[1]);

    move_timer->stop();
    delay_timer->start(300);
}

void Enemy::recalculate_initial_tile(short tile[2]) {

    //Recibe una casilla de la matriz del terreno perteneciente a una hilera exterior (ver documentación del
    //método middle_steps()), y almacena en esta la casilla más cercana a la recibida que se encuentre por
    //fuera de la pantalla, en la región donde aparecen los enemigos.

    short i = short(y()/60), j = short(x()/60);

    if ((i == 1) or (i == 7)) {
        tile[0] = (5*i - 8)/3;
        tile[1] = j;
    }
    else {
        tile[0] = i;
        tile[1] = (7*j - 18)/4;
    }
}

void Enemy::finish_delay() {

    //Recordemos que este slot está conectado al timer delay_timer, el cual es utilizado para implementar un
    //pequeño delay cuando el enemigo colisiona con una roca, por lo cual, llamamos el método update_target()
    //para que el enemigo rote y se recalcule su vector velocidad, para posteriormente llamar el método
    //start() de move_timer para reanudar el movimiento.

    update_target();
    move_timer->start(50);
}

QVector2D Enemy::tiles2pixels(short i, short j) {

    //Recibe los indides de una de las casillas de la matriz del terreno del nivel. Retorna la posición en pixeles
    //del centro de esa casilla según el sistema de coordenadas de la escena.

    return QVector2D(29 + 60*j, 29 + 60*i);
}

QVector2D Enemy::tiles2pixels(QPoint point) {

    //Este es un método sobrecargado.

    return QVector2D(29 + 60*point.y(), 29 + 60*point.x());
}

void Enemy::update_index(short removed_index) {

    //Este slot actualiza el valor de la variable list_index. Esto sucede cuando se elimina algún enemigo de la
    //QList enemies de la clase Level (ver documentación de la clase Level), pues hay un elemento menos, por lo
    //cual, debemos reducir en uno el ínidce en caso de que este se encuentre por encima del índice del
    //elemento eliminado.

    if (list_index > removed_index) list_index--;
}

QRectF Enemy::boundingRect() const {
    return QRectF(-width_half, -height_half, width_half*2, height_half*2);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    //La siguiente sentencia es con el propósito de que se mantenga una buena resolución al realizar rotaciones
    //de la imágden del enemigo, pues sin esta, al rotar la imágen la resolución bajaba mucho.

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->drawPixmap(-width_half, -height_half, *pix);
}

QPainterPath Enemy::shape() const {

    //Colocamos una forma semejante a la imágen del enemigo, esto es para hacer el proceso de las colisiones
    //más consecuente con lo que ve el jugador.

    QPainterPath path;
    if (type == 0) path.addEllipse(QRect(7-width_half, 1-height_half, 22, 34));
    else if (type == 1) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 2) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 3) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 4) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 5) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 6) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else if (type == 7) path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    else path.addRect(QRect(4-width_half, 2-height_half, 32, 36));
    return path;
}
