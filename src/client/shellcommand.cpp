#include "shellcommand.h"
#include <QDebug>

ShellCommand::ShellCommand()
{
}

void ShellCommand::newStdIn(QString s)
{
    qDebug() << "Repeat command: " << s;
}
