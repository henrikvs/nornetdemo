#include "shell.h"
#include <QTextStream>
#include <QString>

Shell::Shell()
{

}

void Shell::start()
{
    QTextStream inStream(stdin);
    QString line;
    do {
        line = inStream.readLine();
        emit newOutput(line);
    } while (line != "exit");
}
