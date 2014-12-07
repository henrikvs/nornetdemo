#include <QCoreApplication>
#include <QTextStream>
#include "relayprog.h"

#include <QThread>
#include <QDebug>
#include "shell.h"
int main(int argc, char *argv[])
{


    if (argc > 1 && QString(argv[1]) == QString("-version")) {
        qDebug() << "version: " << QString::number(RelayProg::VERSION);
        return 1;
    }

    QCoreApplication a(argc, argv);
    RelayProg relayprog;
    QString port;
    if (argc > 1) {
        port = argv[1];
    } else {
        port = "33555";
    }
    qDebug() << argc;
    relayprog.startListening(port.toInt());
    Shell shell;
    QThread thread;
    shell.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &shell, SLOT(start()));
    QObject::connect(&shell, SIGNAL(newOutput(QString)), &relayprog, SLOT(newKeyboardInput(QString)), Qt::QueuedConnection);

    QObject::connect(&shell, SIGNAL(shellClosing()), &thread, SLOT(quit()));
    //QObject::connect(&thread, SIGNAL(finished()), &relayprog, SLOT(shutDown()));
    thread.start();
    qDebug()<< "Started";
    return a.exec();
}
