#include "special_enemies.h"

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CARACOL-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

void Snail::add_glue() {

    //Este método del caracol se utilzia para colocar pegamento sobre la casilla donde se encuentra.

    //Tomamos los índices de la casilla donde se encuentra.

    short i = y()/60, j = x()/60;

    //Es suficiente con verificar que el puntero de la matriz tiles del objeto terrain no sea nulo
    //pues este método no se llamará sobre pegamento de caracol, y nunca estará sobre una roca,
    //es decir, el siguiente condicional sólo se cumplorá si el caracol se encuentra sobre
    //un pegamento colocado por los jugadores, por lo cual lo removemos de la escena
    //para poder colocar el pegamento del caracol.

    if (terrain->tiles[i][j] != nullptr) {
        level->removeItem(terrain->tiles[i][j]);
        delete terrain->tiles[i][j];
    }

    //Agregamos el pegamento del caracol a la matriz tiles de la clase terrain y a la escena.

    terrain->tiles[i][j] = new TerrainObject(i, j, 2);
    level->addItem(terrain->tiles[i][j]);
}

bool Snail::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Esta reimplementación del método collisions_handler() de la clase Enemy, procesa las colisiones del caracol,
    //su funcionamiento básico puede ser consultado en la documentación de este mismo método pero de la clase
    //Enemy. Lo reimplementamos pues el caracol no debe verse afectado por los pegamentos de los jugadores
    //ni los propios (pues ya le colocamos una buena rapidez), además de saber cuando se ha dejado de
    //estar sobre un pegamento propio para colocar otro.

    //Utilizamos la variable in_glue para que, luego de examinar todos los objetos con que el caracol está
    //colisionando, si esta todavía se encuentra en false, significa que el caracol ya no está sobre uno
    //de sus propios pegamentos, por lo cual debemos colocar uno mediante el método add_glue().

    bool in_glue = false;
    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {
            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);

            if ((terrain_object->get_type() == 1) and !QRect(210, 150, 360, 240).contains(x(), y(), false)) {
                rock_collision();
                return true;
            }

            //Notemos que si el anterior condicional sale en true, se retorna el método, pero esto no es un
            //problema porque para que se cumpla su condición tiene que ser que el caracol chocó contra una
            //roca, y para ese momento siempre el pegamento ya habrá sido colocado.

            else if (terrain_object->get_type() == 2) in_glue = true;
        }
        else if (typeid(*item) == typeid(Base)) {

            move_timer->stop();
            emit first_bite();
            bite_timer->start(1000);

            //El comentario anterior aplica de la misma forma para este retorno.

            return true;
        }
    }

    //Como se mencionó, si la variable in_glue continua en false, debemos colocar el pegamento de caracol,
    //no obstante, debemos asegurarnos que el caracol también se encuentre dentro del mapa, para evitar
    //que se acceda a posiciones de la matriz tiles del objeto terrain que no existen, pues los
    //enemigos aparecen fuera de la pantalla.

    if (!in_glue and QRect(0, 0, 780, 540).contains(x(), y(), false)) add_glue();
    return false;
}

//----------------------------------------------------------------------------------------------------------------
//------------------------------------------------PUERCOESPÍN-----------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

bool Porcupine::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Esta reimplementación del método collisions_handler() de la clase Enemy, procesa las colisiones del
    //puercoespín, su funcionamiento básico puede ser consultado en la documentación de este mismo método
    //pero de la clase Enemy. Reimplementamos este método completo y no solamente rock_collision() pues
    //debemos eliminar también la roca, y mientras en este método tenemos un puntero a esta, dentro de
    //rock_collision() no.

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(TerrainObject)) {

            TerrainObject *terrain_object = dynamic_cast<TerrainObject*>(item);
            if ((terrain_object->get_type() == 1) and !QRect(210, 150, 360, 240).contains(x(), y(), false)) {

                //En caso de que el puercoespín colisione con una roca, la eliminamos de la matriz tiles
                //del objeto terrain, la removemos de la escena, liberamos la memoria, reducimos el
                //número de rocas del terreno y llamaos el método reduces_health() para retirar
                //al puercospín

                terrain->tiles[short(terrain_object->y()/60)][short(terrain_object->x()/60)] = nullptr;

                level->removeItem(terrain_object);
                delete terrain_object;
                terrain->rocks_num--;
                reduces_health(max_health);

                return true;
            }
            else if (terrain_object->get_type() == 3) speed = 0.6*speed_aux;
            else if (terrain_object->get_type() == 2) speed = 1.4*speed_aux;
        }
        else if (typeid(*item) == typeid(Base)) {

            move_timer->stop();
            emit first_bite();
            bite_timer->start(1000);

            return true;
        }
    }
    return false;
}

Porcupine::Porcupine(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 4, _level, _terrain, _list_index) {

    //Como el puercoespín debe ir directo hacia la base del jugador, limpiamos su targets por defecto,
    //colocamos como único target a la base y actualizamos su movimiento mediante el método
    //update_target().

    targets.clear();
    targets.enqueue(QVector2D(389, 269));
    update_target();
}

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------LECHUZA-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

bool Owl::collisions_handler(QList<QGraphicsItem *> collisions) {

    //Esta reimplementación del método collisions_handler() de la clase Enemy, procesa las colisiones de la
    //lechuza, su funcionamiento básico puede ser consultado en la documentación de este mismo método
    //pero de la clase Enemy. Reimplementamos este método para que la lechuza sólo colisione con la
    //base.

    for (short i = 0; i < collisions.size(); i++) {
        QGraphicsItem *item = collisions[i];
        if (typeid(*item) == typeid(Base)) {

            move_timer->stop();
            emit first_bite();
            bite_timer->start(1000);
            return true;
        }
    }
    return false;
}

Owl::Owl(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 5, _level, _terrain, _list_index) {

    //Como la lechuza debe ir directo hacia la base pues no colisiona con las rocas, limpiamos sus targets
    //y agregamos unicamente el de la base, además de llamar update_target() para que actualice su
    //movimiento.

    targets.clear();
    targets.enqueue(QVector2D(389, 269));
    update_target();
}

//----------------------------------------------------------------------------------------------------------------
//-------------------------------------------------CAMALEÓN-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

void Chamaleon::camouflage() {

    //Este método se encarga de alternar entre el estado invisible y visible del camaleón.

    //La variable visible indica si el camaleón está invisible o no.

    if (visible) {

        //En caso de que esté visible, colocamos visible en false y removemos al camaleón de la escena,
        //aunque notemos que no detenemos el timer move_timer, por lo cual el camaleón continuará
        //moviendose.

        visible = false;
        if (scene() != nullptr) level->removeItem(this);
    }
    else {

        //En caso de que se encontrara invisible, colocamos visible en true y lo agregamos de nuevo
        //a la escena.

        visible = true;
        level->addItem(this);
    }
}

void Chamaleon::rock_collision() {

    //Esta reimplementación del método rock_collision() de la clase Enemy, se encarga de procesar la colisión del
    //camaléon con una roca, su funcionamiento básico puede ser consultado en la documentación de este mismo
    //método pero de la clase Enemy. Reimplementamos este método porque cuando el camaleón choca con una
    //roca luego de salir de la invisibilidad, o de haberla chocado visible, lo debemos colocar
    //invisible de nuevo y cambiarle la posición.

    //Lo removemos de la escena y colocamos visible en false.

    visible = false;
    if (scene() != nullptr) level->removeItem(this);

    //Escogemos una de 15 casillas posibles para que aparezca y lo colocamos ahí.

    short random_tile = rand()%16, tile[2];

    if (random_tile < 5) setPos(tiles2pixels(1, rand()%5 + 4).toPoint());
    else if (random_tile < 10) setPos(tiles2pixels(7, rand()%5 + 4).toPoint());
    else if (random_tile < 13) setPos(tiles2pixels(rand()%3 + 3, 2).toPoint());
    else setPos(tiles2pixels(rand()%3 + 3, 10).toPoint());

    //Limpiamos los targets y los recalculamos mediante el método recalculate_initial_tile() y set_targets(),
    //posteriormente actualizamos su movimiento mediante update_target() y que debe reiniciar el tiempo
    //de invisibilidad, el cual es controlado por camouflage_timer.

    targets.clear();
    recalculate_initial_tile(tile);
    set_targets(tile[0], tile[1]);
    update_target();

    camouflage_timer->start(3000);
}

void Chamaleon::move() {

    //Esta reimplementación del slot move() de la clase Enemy, se encarga de mover al camaleón, su funcionamiento
    //básico puede ser consultado en la documentación de este mismo slot pero de la clase Enemy. Reimplementamos
    //este slot para agregarle que cuando el camaleón ya este cerca de la base, este se vuelva visible en caso
    //de no serlo, y se quede así, esto es pues como el camaleón se hace invulnerable cuando está invisible
    //como se menciona en special_enemies.h, sería injusto para el jugador que este pueda aparecer ya justo
    //al lado de la base y mucho más golpearla estando invisible, por lo cual agregamos un condicional
    //al slot para evitar esto.

    if (freez) return;

    speed = speed_aux;

    //Si el camaleón ya se encuentra cerca de la base, en caso de que camouflage_timer esté activo, lo detenemos,
    //además de colocar al camaleón visible en caso de no estarlo.

    if (((240 <= x()) and (x() <= 540)) and ((180 <= y()) and (y() <= 360))) {
        if (camouflage_timer->isActive()) camouflage_timer->stop();
        if (!visible) {
            visible = true;
            level->addItem(this);
        }
    }

    if (!collisions_handler(collidingItems(Qt::IntersectsItemBoundingRect))) {

        setPos(x() + 0.06*speed[0], y() + 0.06*speed[1]);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        dir = targets.head() - QVector2D(pos());

        if (dir.length() < 7) {
            targets.dequeue();
            update_target();
        }
    }
}

Chamaleon::Chamaleon(short i, short j, QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(i, j, 6, _level, _terrain, _list_index) {

    //Además de todo el proceso que realiza el constructor de la clase Enemy, en el caso del camaleón también
    //inicializamos la variable visible en true, construimos camouflage_timer, lo conectamos y le llamamos
    //su método start().

    visible = true;
    camouflage_timer = new QTimer;
    connect(camouflage_timer, &QTimer::timeout, this, &Chamaleon::camouflage);
    camouflage_timer->start(3000);
}

Chamaleon::~Chamaleon() {

    //Como reservamos memoria para el timer camouflage_timer, la debemos liberar cuando el camaleón sea
    //derrotado.

    delete camouflage_timer;
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------TOPO--------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

Mole::Mole(QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(0, -1, 7, _level, _terrain, _list_index) {

    //Para el topo, además de los procesos que realiza el constructor de la clase Enemy, le debemos adjuntar
    //unos cuantos más.

    //Pimero no le permitimos moverse pues debemos esperar unos segundos para que salga del agujero.

    move_timer->stop();

    //Escogemos al azar una casilla de 6 muy cercanas a la base.

    short random_tile = rand()%6;

    if (random_tile < 3) tile_pixels = tiles2pixels(rand()%3 + 3, 4);
    else tile_pixels = tiles2pixels(rand()%3 + 3, 8);

    //Colocamos la imágen del agujero.

    hole = new QGraphicsPixmapItem(QPixmap(":/enemies/resources/images/enemies/hole.png"));
    hole->setPos(tile_pixels[0] - 30, tile_pixels[1] - 30);
    level->addItem(hole);
    hole->setZValue(1);

    //Limpiamos los targets y sólo dejamos el de la base.

    targets.clear();
    targets.enqueue(QVector2D(389, 269));

    //Construimos el timer dig_timer, lo conectamos y llamamos su método start(), para que al cabo de un segundo
    //retiremos la imágen del agujero mediante el slot spawn(), además de colocar al topo en la respectiva
    //posición.

    dig_timer = new QTimer;
    dig_timer->setSingleShot(true);
    connect(dig_timer, &QTimer::timeout, this, &Mole::spawn);

    dig_timer->start(1000);
}

void Mole::spawn() {

    //Este slot se encarga de retirar y liberar la memoria utilizada para la imágen el agujero por
    //donde saldrá el topo, además de colocarlo donde estaba anteriormente el agujero y comenzar
    //su movimiento llamado el método start() de move_timer.

    level->removeItem(hole);
    delete hole;

    setPos(tile_pixels[0], tile_pixels[1]);
    update_target();

    move_timer->start(50);
}

Mole::~Mole() {

    //Como reservamos memoria para dig_timer, la debemos liberar cuando el topo
    //sea eliminado.

    delete dig_timer;
}

//----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------BUITRE-------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

Vulture::Vulture(QGraphicsScene *_level, Terrain *_terrain, short _list_index) :
    Enemy(0, -1, 8, _level, _terrain, _list_index) {

    //Además de los procesos que realiza el constructor de Enemy, como el buitre seguirá una trayectoria
    //elíptica, definimos los parámetros de esta y colocamos el buitre en su posición inicial.

    a = 330; b = 210; c = sqrt(a*a + b*b);

    //La variable loop nos indicará cuantas vueltas ha dado el buitre a la base, recordemos que
    //es en la segunda cuando ataca a la base.

    loop_num = 0;

    //Por gusto personal, programé la posición inicial del buitre para que este se pueda mover en una
    //línea recta para llegar al mapa y comenzar con la trayectoria elíptica; esta posición inicial
    //es calculada para que la línea recta por donde se mueve el buitre tenga pendiente 1 y que
    //cuando intersecta la trayectoria elíptica, esta recta sea tangencial a esta para
    //propiciar un cambio suave de trayectoria.

    setPos(389, 269 - c);
    setRotation(-135);

    //La variable offset es utilizada para que cuando el buitre se vaya a dirigir hacia la base, este cambie
    //su trayectoria por otra elipse, pero de menores dimensiones. La variable strike es para saber cuando
    //el buitre ya concluyo las vueltas alrededor de la base y se dispone a ir a atacarla. La variable
    //straight_line inidica cuando el buitre se encuentra siguiendo la trayectoria rectilínea para
    //entrar al mapa y comenzar con la elíptica.

    offset = 0;
    strike = false;
    straight_line = true;

    //Para el movimiento del buitre, tanto el rectilíneo como el elíptico, utilizamos un sistema de referencia
    //cuyo origen se encuentra sobre la base, el semieje positivo de las X crece hacia la derecha y el semieje
    //positivo de las Y crece hacia arriba.

    //Colocamos para el ángulo inicial, respecto al semieje positivo de las X, el del vector que corresponde
    //al punto donde se intersecta la trayectoria rectilína con la elíptica.

    initial_angle = atan2(b*b/c, -a*a/c);
    angle = initial_angle;
}

void Vulture::move() {

    //Esta reimplementación del slot move() de la clase Enemy, se encarga de mover al buitre, su funcionamiento
    //básico puede ser consultado en la documentación de este mismo slot pero de la clase Enemy. Reimplementamos
    //completamente este slot, pues el movimiento del buitre es muy diferente al del resto de enemigos.

    if (freez) return;

    if (straight_line) {

        //Cuando straight_line es true, es porque el buitre está ingresando al mapa siguiendo una trayectoria
        //rectilína, por lo cual utilizamos el modelo del movimiento rectilínea uniforme.

        setPos(x() - 0.1*spd, y() + 0.1*spd);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        //Revisamos si el buitre ya llegó al punto de intersección entre la trayectoria rectilínea y la elíptica,
        //para colocar straight_line en false y comenzar a recorrer esta última.

        if (sqrt(pow((389 - a*a/c) - x(), 2) + pow((269 - b*b/c) - y(), 2)) < 5) {
            straight_line = false;

            //La forma en que el buitre describe la trayectoria elíptica es idéntica a la forma en que lo
            //hacen los power ups, a excepción de que también debemos ir rotando la imágen de este.

            r = radio();
            setPos(389 + r*cos(angle), 269 - r*sin(angle) - offset);
            spd = 300;
        }
    }
    else {

        //Utilizamos derivación implícita sobre la ecuación de la elipse para obtener la pendiente en cualquier
        //punto de esta, según la posición en X e Y, para posteriormente tomar el ángulo de la recta
        //correspondiente y rotar la imágen del buitre.

        setRotation(-90 - atan2(-b*b*(x() - 389), a*a*(269 - y() - offset))*180/M_PI);

        //Como se mencionó, para que el buitre describa la trayectoria elíptica se utiliza el mismo proceso que
        //para los power ups.

        dr = diff_radio();
        angle += 0.06*spd/sqrt(dr*dr + r*r);

        //Si estamos cerca del ángulo 5*pi/2 (debemos usar este y no pi/2 para no tener que hacer dos
        //chequeos del ángulo, uno para cuando supere 2*pi y otro para aumentar la variable
        //loop_num), restamos 2*pi al ángulo (aunque no es necesario pues luego de la
        //segunda vuelta el buitre ataca a la base, sólo lo dejé por seguridad),
        //aumentamos el valor de loop_num pues hemos dado una vuelta y en
        //caso de que sea ya la segunda, modificamos los parámetros a y
        //b de la elipse y el valor de offset para que el buitre se
        //comience a dirigir hacia la base.

        if (abs(angle - 5*M_PI/2) < 0.1) {

            angle -= 2*M_PI;
            loop_num++;

            if (loop_num == 2) {
                a /= 2; b /= 2;
                offset = b;

                //Colocamos strike en true para comenzar a revisar si el buitre ha chocado con
                //la base.

                strike = true;
            }
        }

        //Como se mencionó, para que el buitre describa la trayectoria elíptica se utiliza el mismo proceso que
        //para los power ups.

        r = radio();
        setPos(389 + r*cos(angle), 269 - r*sin(angle) - offset);
        if (health_bar_on) health_bar->setPos(x() - 20, y() - 35);

        //Por último, si strike es true, es porque el buitre ya se dirige hacia la vase, por lo cual
        //debemos comenzar a revisar para detectar cuando colisiona con esta.

        if (strike) collisions_handler(collidingItems(Qt::IntersectsItemBoundingRect));
    }
}

double Vulture::radio() {
    return 1/sqrt(pow(cos(angle)/a, 2) + pow(sin(angle)/b, 2));
}

double Vulture::diff_radio() {
    return (0.5*a*b*(b*b - a*a))*(sin(2*angle)/pow(pow(b*cos(angle), 2) + pow(a*sin(angle), 2) , 1.5));
}

bool Vulture::collisions_handler(QList<QGraphicsItem*> collisions) {

    //Esta reimplementación del método collisions_handler() de la clase Enemy, procesa las colisiones del
    //buitre, su funcionamiento básico puede ser consultado en la documentación de este mismo método
    //pero de la clase Enemy. Reimplementamos este método para evitar que el buitre se vea afectado
    //por las rocas o los pegamentos, además de emitir la señal vulture_hit cuando este choca con
    //la base.

    QGraphicsItem *item;
    for (short i = 0; i < collisions.size(); i++) {
        item = collisions[i];
        if (typeid(*item) == typeid(Base)) {

            //Cuado el buitre choca con la base, emitimos la señal vulture_hit, la cual le informa al objeto
            //base que debe procesar el golpe del buitre, que si no se cuenta con un salvavidas, destruye
            //la base inmediatamente.

            move_timer->stop();
            emit vulture_hit();

            //También eliminamos al buitre pues ya cumplió con su cometido.

            reduces_health(max_health);
            return true;
        }
    }
    return false;
}
