#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T19:21:54
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = demoshell
CONFIG   += console static
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

include(../shell/shell.pri)
include(../libs/libs.pri)
include(../tasks/tasks.pri)
include(../network/network.pri)
include(../globals/globals.pri)
include (../democore/democore.pri)

INCLUDEPATH += ../democore/
INCLUDEPATH += ../shell/
INCLUDEPATH += ../protocol/
INCLUDEPATH += ../libs/
INCLUDEPATH += ../globals/

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)
