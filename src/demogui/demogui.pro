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
        destpath = $$OUT_PWD/debug
        #myfile.path = $$OUT_PWD/debug/tools
    }
    CONFIG(release, debug|release) {
        destpath = $$OUT_PWD/release
        #myfile.path = $$OUT_PWD/release/tools
    }
    LIB_FILES += $$[QT_INSTALL_BINS]/QT5WEBKITWIDGETS.DLL \
                $$[QT_INSTALL_BINS]/QT5WIDGETS.DLL \
                $$[QT_INSTALL_BINS]/QT5NETWORK.DLL \
                $$[QT_INSTALL_BINS]/QT5GUI.DLL \
                $$[QT_INSTALL_BINS]/QT5MULTIMEDIAWIDGETS.DLL \
                $$[QT_INSTALL_BINS]/QT5MULTIMEDIA.DLL \
                $$[QT_INSTALL_BINS]/libGLESv2.dll \
                $$[QT_INSTALL_BINS]/ICUIN51.dll \
                $$[QT_INSTALL_BINS]/ICUUC51.dll \
                $$[QT_INSTALL_BINS]/qt5opengl.dll \
                $$[QT_INSTALL_BINS]/qt5printsupport.dll \
                $$[QT_INSTALL_BINS]/qt5webkit.dll \
                $$[QT_INSTALL_BINS]/qt5sensors.dll \
                $$[QT_INSTALL_BINS]/icudt51.dll \
                $$[QT_INSTALL_BINS]/qt5quick.dll \
                $$[QT_INSTALL_BINS]/QT5CORE.DLL \
                $$[QT_INSTALL_BINS]/DCOMP.DLL \
                $$[QT_INSTALL_BINS]/GPSVC.DLL \
                $$[QT_INSTALL_BINS]/ICUDT51.DLL \
                $$[QT_INSTALL_BINS]/IESHIMS.DLL \
                $$[QT_INSTALL_BINS]/LIBEGL.DLL \
                $$[QT_INSTALL_BINS]/QT5POSITIONING.DLL \
                $$[QT_INSTALL_BINS]/QT5QML.DLL \
                $$[QT_INSTALL_BINS]/QT5SQL.DLL

    platform_plugin.path = $$destpath/platforms
    platform_plugin.files = $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
    message($$platform_plugin.files) #t2
    INSTALLS += platform_plugin

}
linux {
    destpath = $$OUT_PWD
    #myfile.path = $$OUT_PWD/tools
}

#message($$LIB_FILES)
libfiles_install.files = $$LIB_FILES
libfiles_install.path = $$destpath

myfile.path = $$destpath/tools
myfile.files += $$PWD/tools/*

#message($$libfiles_install.files)
INSTALLS += myfile libfiles_install

include (../democore/democore.pri)
include (../tasks/tasks.pri)
include (../utils/utils.pri)
HEADERS += $$files($$PWD/*.h)
SOURCES += $$files($$PWD/*.cpp)
FORMS    += $$files($$PWD/*.ui)

HEADERS += $$files($$PWD/tasks/*.h)
SOURCES += $$files($$PWD/tasks/*.cpp)

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    main.js \
    index.html \
    markerlight.js


#message($$[QT_INSTALL_BINS])#
#QMAKE_EXTRA_TARGETS += expectFile
#POST_TARGETDEPS += expectFile

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/networking/release/ -lnetworking
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/networking/debug/ -lnetworking
else:unix: LIBS += -L$$OUT_PWD/../libs/networking/ -lnetworking

INCLUDEPATH += $$PWD/../libs/networking $$PWD/../libs/networking/networkmessages $$PWD/../libs/networking/sctp
DEPENDPATH += $$PWD/../libs/networking $$PWD/../libs/networking/networkmessages $$PWD/../libs/networking/sctp


#LIBS += -LC:/Users/Henrik/Dropbox/masterthesis/code/build-nornetdemo-Desktop_Qt_5_2_1_MSVC2012_64bit-Release/src/libs/networking/release/ -lnetworking
