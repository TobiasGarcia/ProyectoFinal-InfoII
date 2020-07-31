#include "blackscreen.h"

BlackScreen::BlackScreen() {

    //Colocamos las dimensiones del rectángulo para que cubra toda la escena.

    setRect(0, 0, 779, 599);
    setBrush(Qt::black);
    setZValue(8);

    //Colocamos la opacidad en 100 (esta variable indica el porcentaje de opacidad,
    //en este caso se encuentra al 100%).

    opacity = 100;

    //Construimos el objeto opacity_timer.

    opacity_timer = new QTimer;
    connect(opacity_timer, &QTimer::timeout, this, &BlackScreen::update_opacity);
}

BlackScreen::~BlackScreen() {
    delete opacity_timer;
}

void BlackScreen::change_opacity(bool _increase_opacity) {

    //Este método es utilizado para comenzar a cambiar la opacidad de la pantalla negra gradualmente,
    //según el valor de _increase_opacity, true para indicar que la pantalla se debe colocar
    //(opacidad del 100%), o false cuando esta se debe retirar (opacidad 0).

    increase_opacity = _increase_opacity;
    opacity_timer->start(40);
}

void BlackScreen::update_opacity() {

    //Este slot es activado por el timer opacity_timer, y su porpósito es aumentar o disminuir
    //gradualmente la opacidad de la pantalla negra según el valor de increase_opacity,
    //true para aumentar y false para disminuir.

    if (increase_opacity) opacity += 5;
    else opacity -= 5;

    //En caso de que ya hallamos superado los valores límites, detenemos el timer opacity_timer
    //y en caso de que estemos incrementando la opacidad (colocando la pantalla negra),
    //emitimos la señal finish, para indicar a la escena donde se utilice esta clase
    //que la pantalla negra se ha terminado de colocar.

    if ((opacity < 0) or (100 < opacity)) {
        opacity_timer->stop();
        if (increase_opacity) emit finish();
    }
    else setOpacity(opacity/100.0);
}
