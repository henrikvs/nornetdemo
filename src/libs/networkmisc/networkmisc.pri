HEADERS += $$files($$PWD/*.h)

SOURCES += $$files($$PWD/*.cpp)

for(el, INCLUDEPATH):message(path:$$el)
