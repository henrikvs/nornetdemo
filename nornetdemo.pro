#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T13:07:42
#
#-------------------------------------------------

CONFIG += ordered
TEMPLATE = subdirs

installdirectory = $$OUT_PWD

SUBDIRS = src
test = 5
#include(src/nodeprog/nodeprog.pri)



#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug

#OBJECTS_DIR = ${buildDir}/.obj
#MOC_DIR = ${buildDir}/.moc
#RCC_DIR = ${buildDir}/.qrc
#UI_DIR = ${buildDir}/.ui


nodeproginstall.files = $$PWD/src/nodeprog/install.sh
nodeproginstall.path = $$OUT_PWD/bins/nodeprog


message($$nodeprogpath/install.sh)

#message($$libfiles_install.files)
INSTALLS += nodeproginstall


RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt \
    map.qml
