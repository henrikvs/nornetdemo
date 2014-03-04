#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T17:54:48
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../shell/shell.pri)
include(../tasks/tasks/tasks.pro)
include(../protocol/protocol.pri)
include(../globals/globals.pri)

INCLUDEPATH += ../shell/
INCLUDEPATH += ../protocol/
INCLUDEPATH += ../globals/

DEPENDPATH  += ../shell/

SOURCES += main.cpp \
    shellcommand.cpp \
    clientconnection.cpp \
    client.cpp

HEADERS += \
    shellcommand.h \
    clientconnection.h \
    client.h
