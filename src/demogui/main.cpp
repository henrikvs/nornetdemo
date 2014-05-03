#include "mapoverview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapOverview w;
    w.show();

    return a.exec();
}
