HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)

INCLUDEPATH += src/libs/
INCLUDEPATH += src/tasks/
INCLUDEPATH += src/protocol/
INCLUDEPATH += src/frontend/

DEPENDPATH += src/libs/
