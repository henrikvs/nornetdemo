#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T17:54:48
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = nodeprog
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

include(../libs/libs.pri)

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

