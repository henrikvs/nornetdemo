INCLUDEPATH += $$PWD/networkmessages $$PWD


HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

include(networkmessages/networkmessages.pri)
