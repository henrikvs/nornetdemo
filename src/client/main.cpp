#include <QCoreApplication>
#include <QTextStream>
#include "client.h"
#include "shell.h"
#include "shellcommand.h"
#include "serverprotocol.h"
#include <QThread>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    Shell shell;
    QThread thread;
    ShellCommand command;
    shell.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &shell, SLOT(start()));
    QObject::connect(&shell, SIGNAL(newOutput(QString)), &client, SLOT(newStdIn(QString)), Qt::QueuedConnection);

    QObject::connect(&shell, SIGNAL(shellClosing()), &thread, SLOT(quit()));
    QObject::connect(&shell,SIGNAL(shellClosing()), &client, SLOT(shutDown()));
    thread.start();
    qDebug()<< "Started";
    return a.exec();
}
