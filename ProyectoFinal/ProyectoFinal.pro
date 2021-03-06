QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game/blackscreen.cpp \
    game/game.cpp \
    game/information.cpp \
    level/base.cpp \
    level/enemy.cpp \
    level/level.cpp \
    level/powerup.cpp \
    level/special_enemies.cpp \
    level/terrain.cpp \
    level/terrainobject.cpp \
    levels_menu/levelsmenu.cpp \
    levels_menu/needle.cpp \
    main.cpp \
    minigame/ball.cpp \
    minigame/minigame.cpp \
    player/fireball.cpp \
    player/player.cpp \
    widget.cpp

HEADERS += \
    game/blackscreen.h \
    game/game.h \
    game/information.h \
    level/base.h \
    level/enemy.h \
    level/level.h \
    level/powerup.h \
    level/special_enemies.h \
    level/terrain.h \
    level/terrainobject.h \
    levels_menu/levelsmenu.h \
    levels_menu/needle.h \
    minigame/ball.h \
    minigame/minigame.h \
    player/fireball.h \
    player/player.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = galaxy_lacters.ico
