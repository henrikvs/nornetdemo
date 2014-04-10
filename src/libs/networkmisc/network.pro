TEMPLATE = lib

QT += core network
INCLUDEPATH += $$PWD
include(protocols/protocols.pri)
include(messages/messages.pri)

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)



#include(messages/messages.pri)

#INCLUDEPATH += messages/ $$PWD/protocols/ $$PWD/../democore/
#message($$PWD/protocols/)
