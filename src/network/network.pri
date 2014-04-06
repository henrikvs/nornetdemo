HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

INCLUDEPATH += ../tasks/

INCLUDEPATH += messages/ $$PWD/protocols/

include(messages/messages.pri)
include(protocols/protocols.pri)
