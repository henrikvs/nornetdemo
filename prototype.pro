#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T13:07:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prototype
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interactiveopen.cpp \
    sslform.cpp \
    dialog.cpp \
    experiment.cpp \
    experimentupdate.cpp \
    nodeupdate.cpp \
    node.cpp \
    communicator.cpp \
    connection.cpp \
    connectionupdate.cpp \
    shell.cpp \
    sshconnection.cpp

HEADERS  += mainwindow.h \
    interactiveopen.h \
    sslform.h \
    dialog.h \
    experiment.h \
    experimentupdate.h \
    nodeupdate.h \
    node.h \
    communicator.h \
    connection.h \
    connectionupdate.h \
    shell.h \
    sshconnection.h

FORMS    += mainwindow.ui \
    sslform.ui \
    dialog.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    notes.txt
