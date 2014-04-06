HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)

INCLUDEPATH += src/libs/
INCLUDEPATH += src/tasks/
INCLUDEPATH += src/network/ src/network/messages/
INCLUDEPATH += src/democore/

DEPENDPATH += src/libs/
