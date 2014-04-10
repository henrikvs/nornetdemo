QT += testlib
QT -= gui
TARGET = Test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

include (libs/lib_tests.pri)
include (../src/libs/libs.pri)
