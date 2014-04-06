#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T17:54:48
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = nodeprog
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

include(../shell/shell.pri)
include(../libs/libs.pri)
include(../tasks/tasks.pri)
include(../network/network.pri)
include(../globals/globals.pri)

INCLUDEPATH += ../shell/
INCLUDEPATH += ../libs/
INCLUDEPATH += ../globals/

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

