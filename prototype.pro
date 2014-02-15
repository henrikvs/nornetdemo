#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T13:07:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prototype
TEMPLATE = app

include(src/libs/libs.pri)
include(src/GUI/GUI.pri)
include(src/shell/shell.pri)

RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt
