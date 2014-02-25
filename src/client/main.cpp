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
    Client *client = new Client();
    Shell shell;
    QThread thread;
    ShellCommand command;
    shell.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &shell, SLOT(start()));
    QObject::connect(&shell, SIGNAL(newOutput(QString)), &command, SLOT(newStdIn(QString)));
    thread.start();
    qDebug()<< "Started";
    return a.exec();
}
