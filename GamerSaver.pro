QT += core network widgets
TEMPLATE = app
TARGET = GamerSaver
MAKEFILE = QtMakefile
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++14

INCLUDEPATH = include
HEADERS = include/*.h
SOURCES = src/*.cpp

OBJECTS_DIR = obj
MOC_DIR = obj

RESOURCES = res/resources.qrc
RCC_DIR = obj
DESTDIR = bin

install_gameList.path = $$DESTDIR
install_gameList.files += $$PWD/res/GameList.ini
INSTALLS += install_gameList
