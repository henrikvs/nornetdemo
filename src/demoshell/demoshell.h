#ifndef DEMOSHELL_H
#define DEMOSHELL_H
#include "shell.h"
#include <QObject>
#include <QThread>
#include "sliver.h"
#include "pingreply.h"
#include "nodeinfomessage.h"

class DemoCore;

class DemoShell : public QObject
{
    Q_OBJECT
public:
    explicit DemoShell(DemoCore *core, QObject *parent = 0);
    void run();

private:
    DemoCore *core;
    Shell shell;
    QThread thread;

signals:

public slots:
    void shutDown();
    void handleStatusMessage(Sliver sliver, NodeInfoMessage message);
    void handlePingReply(Sliver sliver, PingReply message);
    void newStdIn(QString input);
};

#endif // DEMOSHELL_H
