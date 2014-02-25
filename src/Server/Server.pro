#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T16:54:51
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    tcpconnection.cpp

HEADERS += \
    server.h \
    tcpconnection.h
