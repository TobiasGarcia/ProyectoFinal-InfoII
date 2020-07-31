#include "information.h"

Information::Information(QGraphicsScene *_target_scene) : target_scene(_target_scene) {

    //El puntero _target_scene apunta a la escena donde se colocará el mensaje, esto es
    //para poder que este se coloque solo con llamar al método display_message() de
    //esta clase.

    //Construimos el objeto text, le colocamos color y el tipo y tamaño de letra.

    text = new QGraphicsTextItem(this);

    text->setDefaultTextColor(Qt::white);
    text->setFont(QFont("Kepler 296", 16));

    //El método setPlainText() de la clase QGraphicsTextItem tarda mucho en ejecutarse la primera
    //vez que es invocado, por lo cual colocamos un texto cualesquiera para que el método
    //setPlainText() no tarde tanto cuando se llame dentro del método display_message().

    text->setPlainText("Default");

    //El puntero pix lo utilizamos cuando además de texto queremos colocar una imágen
    //junto con el.

    pix = nullptr;

    //Colocamos el color del rectángulo de fondo.

    setBrush(QColor(104, 109, 117));
    setPen(QPen(QColor(224, 162, 43), 5));
    setZValue(7);

    //Construimos el objeto display_timer.

    display_timer = new QTimer;
    display_timer->setSingleShot(true);
    connect(display_timer, &QTimer::timeout, this, &Information::remove);
}

Information::~Information() {
    delete text;
    delete display_timer;
    if (pix != nullptr) delete pix;
}

void Information::display_message(short x, short y, QString message) {

    //Este método se encarga de mostrarle el mensaje almacenado en el QString message al usuario;
    //el mensaje es mostrado dentro de un rectángulo, los valores de que x e y indican la
    //posición del punto medio del borde superior de este.

    //Colocamos el mensaje.

    text->setPlainText(message);

    //En caso de que previamente hayamos colocado una imágen, la eliminamos.

    if (pix != nullptr) {
        delete pix;
        pix = nullptr;
    }

    //Definimos la posición y las dimensiones del rectángulo que contendrá el text de acuerdo
    //al tamaño de este.

    setRect(-10, -5, text->boundingRect().width() + 20, text->boundingRect().height() + 6);
    setPos(x - text->boundingRect().width()/2, y);

    //Notemos que para esto es que necesitamos target_scene, para poder colocar el texto en caso
    //de que este aún no se encuentre en ninguna escena.

    if (scene() == nullptr) target_scene->addItem(this);
}

void Information::display_message(short x, short y, QString message, QString image_path) {

    //Este es un método sobre cargado, es llamado cuando se desea colocar una imágen junto con
    //el texto; la variable image_path almacena la ruta de dicha imágen.

    text->setPlainText(message);

    if (pix != nullptr) delete pix;

    //Construimos el objeto pix con la imágen solicitada.

    pix = new QGraphicsPixmapItem(QPixmap(image_path).transformed(QTransform().rotate(90)), this);

    //Colocamos la imágen según el tamaño el texto.

    pix->setOffset(-pix->boundingRect().width()/2, 0);
    pix->setPos(text->boundingRect().width()/2, -pix->boundingRect().height() - 5);

    //Colocamos las dimensiones del rectángulo de fondo en dependiendo del tamaño del texto y
    //de la imágen.

    setRect(-10, -pix->boundingRect().height() - 15, text->boundingRect().width() + 20,
            pix->boundingRect().height() + text->boundingRect().height() + 21);
    setPos(x - text->boundingRect().width()/2, y);

    if (scene() == nullptr) target_scene->addItem(this);
}

void Information::set_display_time(unsigned long long int millis) {

    //Este método se encarga de llamar el método start() de display_timer evaluado en el valor de millis,
    //esto es con el propósito de que el mensaje sólo se muestre en pantalla por el tiempo indicado por
    //millis.

    display_timer->start(millis);
}

void Information::remove() {

    //Este slot es llamado por el timer display_timer, y su propósito es remover
    //el mensaje de la escena.

    target_scene->removeItem(this);
}
