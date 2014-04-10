TEMPLATE = lib

QT += core network

include(protocols/protocols.pri)
include(messages/messages.pri)


HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)



#include(messages/messages.pri)

#INCLUDEPATH += messages/ $$PWD/protocols/ $$PWD/../democore/
#message($$PWD/protocols/)
