TEMPLATE = lib
QT += core network


HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)

LIBS += -L$$OUT_PWD/../libs/release/libs.dll
INCLUDEPATH += $$PWD/../libs/networkmisc $$PWD/../libs/networkprotocols $$PWD/../libs/networkmessages $$PWD/../libs/utils
message(ok)
for(el, INCLUDEPATH):message($${el})
