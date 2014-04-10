#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T13:07:42
#
#-------------------------------------------------

CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = src
test = 5
#include(src/nodeprog/nodeprog.pri)



#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug

#OBJECTS_DIR = ${buildDir}/.obj
#MOC_DIR = ${buildDir}/.moc
#RCC_DIR = ${buildDir}/.qrc
#UI_DIR = ${buildDir}/.ui



RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt \
    map.qml
