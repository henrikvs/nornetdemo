TEMPLATE = lib

QT += core network

INCLUDEPATH += networkmisc tasks utils networkprotocols networkmessages
include(networkmisc/networkmisc.pri)
include(tasks/tasks.pri)
include(utils/utils.pri)
include(networkprotocols/networkprotocols.pri)
include(networkmessages/networkmessages.pri)

HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)
