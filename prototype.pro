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



#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug

#OBJECTS_DIR = ${buildDir}/.obj
#MOC_DIR = ${buildDir}/.moc
#RCC_DIR = ${buildDir}/.qrc
#UI_DIR = ${buildDir}/.ui



RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt
