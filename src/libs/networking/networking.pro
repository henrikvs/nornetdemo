QT       += core network
INCLUDEPATH += $$PWD/networkmessages $$PWD
CONFIG += c++11
TEMPLATE = lib

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)

DEFINES += NETWORKLIB

include(networkmessages/networkmessages.pri)
