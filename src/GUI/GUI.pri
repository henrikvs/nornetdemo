HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)

INCLUDEPATH += src/libs/
INCLUDEPATH += src/tasks/
INCLUDEPATH += src/protocol/

DEPENDPATH += src/libs/
