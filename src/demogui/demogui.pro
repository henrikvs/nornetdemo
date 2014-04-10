QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = demogui
CONFIG += c++11

include(../libs/libs.pri)
include (../democore/democore.pri)

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)


