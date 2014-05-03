#123
QT       += core gui network webkit webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = demogui
CONFIG += c++11
#LIBS += -lsctp

expectFile.path = $$OUT_PWD
expectFile.files += ssh-add.exp
INSTALLS += expectFile

include(../libs/libs.pri)
include (../democore/democore.pri)
include (../libs/qcustomplot/qcustomplot.pri)

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)
message($$[QT_INSTALL_LIBS])
#message($$OUT_PWD)
#message($$PWD/ssh-add.exp)
RESOURCES += \
    resources.qrc

OTHER_FILES += \
    main.js \
    index.html \
    markerlight.js


