#ifndef POWERUP_H
#define POWERUP_H

#include <math.h>

#include <QTimer>
#include <QPainter>
#include <QGraphicsPixmapItem>

class PowerUp: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

    //Para permitir que los power ups describan alguna de las diferentes trayectorias: una elipse, un astroide,
    //una rosa polar de 5 pétalos o una lemniscata de Bernoulli, bastaría con utilizar su ecuación en coordenadas
    //polares, y la relación entre estas y las cartesianas (x = r(a)*cos(a), y = r(a)*sin(a)), comenzando a
    //aumentar el valor del ángulo para que se describa la curva, sin embargo, como el ángulo crecería a un ritmo
    //fijo pero la longitud de arco que se recorre en la curva es variante, la rapidez con que se describe la
    //trayectoria no es constante, y en lo personal no me gustaba como se veía; el problema radicaba en la forma
    //en que ángulo debía incrementar, es decir, debíamos calcular y utilizar su derivada temporal, por lo cual,
    //luego de unos pequeños cálculos, se puede mostrar que para manetener una rapidez constante V, la derivada
    //del ángulo respecto al tiempo debe ser da/dt = V/sqrt((r^2 + (dr/da)^2), donde r es r(a), la ecuación en
    //coordenadas polares de la curva evaluada en el ángulo, y dr/da es su derivada respecto al ángulo evaluada
    //en este mismo.

    //Como podemos notar, la derivada temporal del ángulo no depende del tiempo, por lo cual podemos prescindir
    //de una variable temporal; finalmente, al aproximar la derivada da/dt a Da/Dt, donde Dt es el intervalo de
    //tiempo entre cada actualización de la posición del objeto, y Da es el cambio en el ángulo durante dicho
    //intervalo Dt, se concluye que para mantener una rapidez constante basta aumentar el ángulo en
    //Dt*V/sqrt((r^2 + (dr/da)^2) radianes.

    //NOTA IMPORTANTE: Todos los cálculos de las trayecotorias son realizados en pixeles, pero tomando como
    //si el origen estuviera en el centro del mapa (x = 389, y = 269), no el origen de la escena ni tampoco
    //el centro de esta (x = 389, y = 299), pues el HUD nos gasta 60 pixeles de la escena. El cambio de
    //sistemas de coordenadas se hace en los argumentos del método setPos() para ser consecuentes.

private:
    QPixmap *pix;
    QTimer *move_timer;
    bool straight_line, freez;
    short spd, path_type, power_type;

    //NOTA: La variable spd será la rapidez con que se moverá el power up, en pixeles por segundo.

    //Las variables a y b son los parámetros de las diferentes trayectorias; tipo float para que cuando se
    //utilicen en un operación de divisón no se realice una división entera, aunque no ameritaban ser
    //tipo double pues son paranmétros fijos, los utilizaremos para los cálculos pero no para
    //almacenar resultados.

    float a, b;

    //Las siguietnes dos variables, c y s, con utilizadase exclusivamente en el cálculo de la
    //derivada del astroide, resultan útiles pues en la fórmula aparece muchas veces las
    //funciones seno y coseno, por lo cual calculamos sus valores sobre el ángulo actual
    //para sólo tener que realizar lecturas de estos valores en lugar de recalcularlas
    //cada vez.

    //En dr calcularemos dr/da evaluado en el ángulo angle; en r calcularemos la función de la curva en
    //coordenadas polares evaluada en el ángulo angle; m_angle es una variable auxiliar utilzada en el
    //cálculo de la lemniscata de Bernoulli, pues esta trayectoria fue la más difícil de simular ya
    //que la ecuación de la curva completa no es una función; y period es el período de la curva,
    //esto es para restárselo cuando este lo supere y así evitar que el ángulo aumente
    //indefinidamente. El valor de angle y m_angle se encuentra en radianes.

    double angle, r, dr, m_angle, period, s, c;

    double radio();
    double diff_radio();

    QPainterPath shape() const;

    void define_parameters();

public:
    QTimer *display_timer;

    void emit_give_power();
    void set_freez(bool _freez) {freez = _freez;};

    PowerUp(short _path_type, short _power_type);
    ~PowerUp();

public slots:
    void move();
    void lemniscate_move();

signals:
    void give_power(short power_type);
};

#endif // POWERUP_H
