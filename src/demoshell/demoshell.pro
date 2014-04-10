#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T19:21:54
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = demoshell
CONFIG   += console c++11
CONFIG   -= app_bundle
#QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

include(../libs/libs.pri)
include (../democore/democore.pri)

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)
