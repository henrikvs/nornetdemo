#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T17:54:48
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = relayprog
CONFIG   += console c++11
CONFIG   -= app_bundle
#LIBS += -lsctp
TEMPLATE = app


linux {
    relayprogpath = $$OUT_PWD/src/relayprog
    #myfile.path = $$OUT_PWD/tools
}

target.path = $$OUT_PWD/../../bins/relayprog

include (../utils/utils.pri)

INSTALLS += target

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/networking/release/ -lnetworking
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/networking/debug/ -lnetworking
else:unix: LIBS += -L$$OUT_PWD/../libs/networking/ -lnetworking

INCLUDEPATH += $$PWD/../libs/networking $$PWD/../libs/networking/networkmessages
DEPENDPATH += $$PWD/../libs/networking $$PWD/../libs/networking/networkmessage
