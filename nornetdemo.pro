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

linux {
    demoguipath = $$OUT_PWD/src/demogui
    nodeprogpath = $$OUT_PWD/src/nodeprog
    relayprogpath = $$OUT_PWD/src/relayprog
    #myfile.path = $$OUT_PWD/tools
}

demoguifiles.files = $$demoguipath/demogui
demoguifiles.path = bins/demogui

nodeprogfiles.files += $$nodeprogpath/nodeprog
nodeprogfiles.path = bins/nodeprog

nodeproginstall.files = $$PWD/src/nodeprog/install.sh
nodeproginstall.path = bins/nodeprog

relayprogfiles.files = $$relayprogpath/relayprog
relayprogfiles.path = bins/relayprog

message($$nodeprogpath/install.sh)

#message($$libfiles_install.files)
INSTALLS += demoguifiles nodeprogfiles relayprogfiles nodeproginstall


RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt \
    map.qml
