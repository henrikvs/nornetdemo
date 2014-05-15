#include "mapoverview.h"
#include "sctptestdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //SctpTestDialog dialog;
    //dialog.show();
    MapOverview w;

    QObject::connect(&a, &QApplication::aboutToQuit, &w, &MapOverview::handleAboutToQuit);
    w.show();

    return a.exec();
}
