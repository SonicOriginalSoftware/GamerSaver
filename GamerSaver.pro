# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = GamerSaver
QT = widgets network

HEADERS = \
   $$PWD/source/include/game.h \
   $$PWD/source/include/mainwindow.h \
   $$PWD/source/include/networkmanager.h \
   $$PWD/source/include/oauth.h

SOURCES = \
   $$PWD/source/src/game.cpp \
   $$PWD/source/src/main.cpp \
   $$PWD/source/src/mainwindow.cpp \
   $$PWD/source/src/networkmanager.cpp \
   $$PWD/source/src/oauth.cpp \

OTHER_FILES = \
    $$PWD/README.md \
    $$PWD/meson.build \
    $$PWD/source/meson.build \
    $$PWD/source/res/*


INCLUDEPATH = \
    $$PWD/source/include

#DEFINES = 
