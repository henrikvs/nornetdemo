#include <QCoreApplication>
#include "shell.h"
#include "demoshell.h"
#include "democore.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DemoCore core;
    DemoShell demoshell(&core);
    demoshell.run();
    return a.exec();
}
