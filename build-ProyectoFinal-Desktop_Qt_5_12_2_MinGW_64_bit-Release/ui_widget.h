/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stacked_w;
    QWidget *main_menu;
    QPushButton *new_btn;
    QPushButton *load_btn;
    QPushButton *exit_btn;
    QWidget *new_log;
    QPushButton *return_new_btn;
    QPushButton *register_btn;
    QCheckBox *pass_cbox;
    QLabel *info_lb;
    QPushButton *mode_btn;
    QLabel *id_lb;
    QLineEdit *id_line;
    QLineEdit *pass_line;
    QLineEdit *pass2_line;
    QLabel *pass_lb;
    QLabel *pass2_lb;
    QWidget *log;
    QPushButton *return_log_btn;
    QPushButton *load_game_btn;
    QLabel *id_log_lb;
    QLabel *pass_log_lb;
    QLineEdit *id_log_line;
    QLineEdit *pass_log_line;
    QLabel *info_log_lb;
    QCheckBox *pass_log_cbox;
    QWidget *game;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *game_gv;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(800, 600);
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        stacked_w = new QStackedWidget(Widget);
        stacked_w->setObjectName(QString::fromUtf8("stacked_w"));
        main_menu = new QWidget();
        main_menu->setObjectName(QString::fromUtf8("main_menu"));
        new_btn = new QPushButton(main_menu);
        new_btn->setObjectName(QString::fromUtf8("new_btn"));
        new_btn->setGeometry(QRect(295, 348, 190, 50));
        QFont font;
        font.setPointSize(22);
        new_btn->setFont(font);
        new_btn->setCursor(QCursor(Qt::PointingHandCursor));
        load_btn = new QPushButton(main_menu);
        load_btn->setObjectName(QString::fromUtf8("load_btn"));
        load_btn->setGeometry(QRect(290, 434, 200, 50));
        load_btn->setFont(font);
        load_btn->setCursor(QCursor(Qt::PointingHandCursor));
        exit_btn = new QPushButton(main_menu);
        exit_btn->setObjectName(QString::fromUtf8("exit_btn"));
        exit_btn->setGeometry(QRect(350, 520, 80, 44));
        exit_btn->setFont(font);
        exit_btn->setCursor(QCursor(Qt::PointingHandCursor));
        stacked_w->addWidget(main_menu);
        new_log = new QWidget();
        new_log->setObjectName(QString::fromUtf8("new_log"));
        return_new_btn = new QPushButton(new_log);
        return_new_btn->setObjectName(QString::fromUtf8("return_new_btn"));
        return_new_btn->setGeometry(QRect(640, 520, 100, 50));
        return_new_btn->setFont(font);
        return_new_btn->setCursor(QCursor(Qt::PointingHandCursor));
        register_btn = new QPushButton(new_log);
        register_btn->setObjectName(QString::fromUtf8("register_btn"));
        register_btn->setGeometry(QRect(420, 520, 170, 50));
        register_btn->setFont(font);
        register_btn->setCursor(QCursor(Qt::PointingHandCursor));
        pass_cbox = new QCheckBox(new_log);
        pass_cbox->setObjectName(QString::fromUtf8("pass_cbox"));
        pass_cbox->setGeometry(QRect(550, 245, 201, 31));
        QFont font1;
        font1.setPointSize(16);
        pass_cbox->setFont(font1);
        pass_cbox->setCursor(QCursor(Qt::PointingHandCursor));
        info_lb = new QLabel(new_log);
        info_lb->setObjectName(QString::fromUtf8("info_lb"));
        info_lb->setGeometry(QRect(310, 310, 431, 171));
        QFont font2;
        font2.setPointSize(18);
        info_lb->setFont(font2);
        info_lb->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        mode_btn = new QPushButton(new_log);
        mode_btn->setObjectName(QString::fromUtf8("mode_btn"));
        mode_btn->setGeometry(QRect(40, 290, 221, 61));
        mode_btn->setFont(font);
        mode_btn->setCursor(QCursor(Qt::PointingHandCursor));
        id_lb = new QLabel(new_log);
        id_lb->setObjectName(QString::fromUtf8("id_lb"));
        id_lb->setGeometry(QRect(120, 61, 341, 31));
        id_lb->setFont(font);
        id_line = new QLineEdit(new_log);
        id_line->setObjectName(QString::fromUtf8("id_line"));
        id_line->setGeometry(QRect(460, 59, 181, 30));
        QFont font3;
        font3.setPointSize(20);
        id_line->setFont(font3);
        pass_line = new QLineEdit(new_log);
        pass_line->setObjectName(QString::fromUtf8("pass_line"));
        pass_line->setGeometry(QRect(460, 118, 181, 30));
        QFont font4;
        font4.setPointSize(12);
        pass_line->setFont(font4);
        pass_line->setEchoMode(QLineEdit::Password);
        pass2_line = new QLineEdit(new_log);
        pass2_line->setObjectName(QString::fromUtf8("pass2_line"));
        pass2_line->setGeometry(QRect(460, 177, 181, 30));
        pass2_line->setFont(font4);
        pass2_line->setEchoMode(QLineEdit::Password);
        pass_lb = new QLabel(new_log);
        pass_lb->setObjectName(QString::fromUtf8("pass_lb"));
        pass_lb->setGeometry(QRect(270, 120, 191, 31));
        pass_lb->setFont(font);
        pass2_lb = new QLabel(new_log);
        pass2_lb->setObjectName(QString::fromUtf8("pass2_lb"));
        pass2_lb->setGeometry(QRect(132, 179, 331, 31));
        pass2_lb->setFont(font);
        stacked_w->addWidget(new_log);
        log = new QWidget();
        log->setObjectName(QString::fromUtf8("log"));
        return_log_btn = new QPushButton(log);
        return_log_btn->setObjectName(QString::fromUtf8("return_log_btn"));
        return_log_btn->setGeometry(QRect(640, 520, 100, 50));
        return_log_btn->setFont(font);
        return_log_btn->setCursor(QCursor(Qt::PointingHandCursor));
        load_game_btn = new QPushButton(log);
        load_game_btn->setObjectName(QString::fromUtf8("load_game_btn"));
        load_game_btn->setGeometry(QRect(380, 520, 211, 50));
        load_game_btn->setFont(font);
        load_game_btn->setCursor(QCursor(Qt::PointingHandCursor));
        id_log_lb = new QLabel(log);
        id_log_lb->setObjectName(QString::fromUtf8("id_log_lb"));
        id_log_lb->setGeometry(QRect(120, 55, 341, 31));
        id_log_lb->setFont(font);
        pass_log_lb = new QLabel(log);
        pass_log_lb->setObjectName(QString::fromUtf8("pass_log_lb"));
        pass_log_lb->setGeometry(QRect(270, 115, 191, 31));
        pass_log_lb->setFont(font);
        id_log_line = new QLineEdit(log);
        id_log_line->setObjectName(QString::fromUtf8("id_log_line"));
        id_log_line->setGeometry(QRect(460, 53, 181, 30));
        id_log_line->setFont(font3);
        pass_log_line = new QLineEdit(log);
        pass_log_line->setObjectName(QString::fromUtf8("pass_log_line"));
        pass_log_line->setGeometry(QRect(460, 113, 181, 30));
        pass_log_line->setFont(font3);
        pass_log_line->setEchoMode(QLineEdit::Password);
        info_log_lb = new QLabel(log);
        info_log_lb->setObjectName(QString::fromUtf8("info_log_lb"));
        info_log_lb->setGeometry(QRect(310, 260, 431, 171));
        info_log_lb->setFont(font2);
        info_log_lb->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        pass_log_cbox = new QCheckBox(log);
        pass_log_cbox->setObjectName(QString::fromUtf8("pass_log_cbox"));
        pass_log_cbox->setGeometry(QRect(560, 175, 201, 31));
        pass_log_cbox->setFont(font1);
        pass_log_cbox->setCursor(QCursor(Qt::PointingHandCursor));
        stacked_w->addWidget(log);
        game = new QWidget();
        game->setObjectName(QString::fromUtf8("game"));
        horizontalLayout_2 = new QHBoxLayout(game);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        game_gv = new QGraphicsView(game);
        game_gv->setObjectName(QString::fromUtf8("game_gv"));

        horizontalLayout_2->addWidget(game_gv);

        stacked_w->addWidget(game);

        horizontalLayout->addWidget(stacked_w);


        retranslateUi(Widget);

        stacked_w->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        new_btn->setText(QApplication::translate("Widget", "Juego Nuevo", nullptr));
        load_btn->setText(QApplication::translate("Widget", "Cargar Partida", nullptr));
        exit_btn->setText(QApplication::translate("Widget", "Salir", nullptr));
        return_new_btn->setText(QApplication::translate("Widget", "Atr\303\241s", nullptr));
        register_btn->setText(QApplication::translate("Widget", "\302\241Comenzar!", nullptr));
        pass_cbox->setText(QApplication::translate("Widget", "Ver contrase\303\261a", nullptr));
        info_lb->setText(QString());
        mode_btn->setText(QApplication::translate("Widget", "Un solo Jugador", nullptr));
        id_lb->setText(QApplication::translate("Widget", "Nombre de la Partida:", nullptr));
        pass_lb->setText(QApplication::translate("Widget", "Contrase\303\261a:", nullptr));
        pass2_lb->setText(QApplication::translate("Widget", "Repite la Contrase\303\261a:", nullptr));
        return_log_btn->setText(QApplication::translate("Widget", "Atr\303\241s", nullptr));
        load_game_btn->setText(QApplication::translate("Widget", "Cargar Partida", nullptr));
        id_log_lb->setText(QApplication::translate("Widget", "Nombre de la Partida:", nullptr));
        pass_log_lb->setText(QApplication::translate("Widget", "Contrase\303\261a:", nullptr));
        info_log_lb->setText(QString());
        pass_log_cbox->setText(QApplication::translate("Widget", "Ver contrase\303\261a", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
