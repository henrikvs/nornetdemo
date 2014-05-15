QT       += core network
INCLUDEPATH += $$PWD
CONFIG += c++11
TEMPLATE = lib
LIBS += -lsctp

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)

DEFINES += NETWORKLIB

include(networkmessages/networkmessages.pri)
include(sctp/sctp.pri)
