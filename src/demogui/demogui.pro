#123
QT       += core gui network webkit webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
TARGET = demogui
CONFIG += c++11
#LIBS += -lsctp
#$$OUT_PWD/


win32 {
    CONFIG(debug, debug|release) {
        myfile.path = $$OUT_PWD/debug/tools
    }
    CONFIG(release, debug|release) {
        myfile.path = $$OUT_PWD/release/tools
    }
}
linux {
    myfile.path = $$OUT_PWD/tools
}

myfile.files += $$PWD/tools/*


INSTALLS += myfile


include(../libs/libs.pri)
include (../democore/democore.pri)
include (../libs/qcustomplot/qcustomplot.pri)

HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    main.js \
    index.html \
    markerlight.js



message($$[QT_INSTALL_DATA])#
#QMAKE_EXTRA_TARGETS += expectFile
#POST_TARGETDEPS += expectFile
