#include "shell.h"
#include <QTextStream>
#include <QString>
#include <QDebug>
Shell::Shell(QString terminate) : terminate(terminate)
{

}

void Shell::start()
{
    QTextStream inStream(stdin);
    QString line;
    do {
        line = inStream.readLine();
        emit newOutput(line);
    } while (line != terminate);
    qDebug() << "Exiting shell";
    emit shellClosing();
}
