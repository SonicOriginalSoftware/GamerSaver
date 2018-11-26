QT += core gui network networkauth widgets
TEMPLATE = app
TARGET = GamerSaver

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

INCLUDEPATH = include
SOURCES = src/*.cpp
HEADERS = include/*.h
FORMS = src/*.ui

OBJECTS_DIR = _obj
MOC_DIR = _obj
UI_DIR = _ui

DESTDIR = bin
MAKEFILE = QtMakefile
