#include "demoshell.h"
#include "shell.h"
#include "democore.h"
#include <QNetworkInterface>
DemoShell::DemoShell(DemoCore *core, QObject *parent) :
    QObject(parent), core(core)
{
}

void DemoShell::run()
{
    shell.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &shell, SLOT(start()));
    QObject::connect(&shell, SIGNAL(newOutput(QString)), this, SLOT(newStdIn(QString)), Qt::QueuedConnection);

    QObject::connect(&shell, SIGNAL(shellClosing()), &thread, SLOT(quit()));
    QObject::connect(&thread,SIGNAL(finished()), core, SLOT(shutDown()));
    QObject::connect(core, SIGNAL(shutDownComplete(int)), this, SLOT(shutDown()));

    connect(core, SIGNAL(newPingReply(Sliver,PingReply)), this, SLOT(handlePingReply(Sliver,PingReply)));
    connect(core, SIGNAL(newStatusMessage(Sliver,NodeInfoMessage)), this, SLOT(handleStatusMessage(Sliver,NodeInfoMessage)));

    thread.start();
    core->start();
    //core.connectToSlivers();
}

void DemoShell::shutDown() {
    exit(NetworkEntity::EXIT_TYPE_NORMAL);
}

void DemoShell::handleStatusMessage(Sliver sliver, NodeInfoMessage message)
{
    qDebug() << "New status message from: " << sliver.name;
}

void DemoShell::handlePingReply(Sliver sliver, PingReply message)
{
    qDebug() << "New ping reply message from: " << sliver.name;
}

void DemoShell::newStdIn(QString input)
{
    QStringList tokens = input.split(QRegExp("\\s"));
    if (tokens[0] == "listen") {
        if (tokens.length() == 3) {
            core->startListening(tokens[1].toInt(), tokens[2].toInt());
        } else  {
            core->startListening(tokens[1].toInt());
        }
    } else if (tokens[0] == "listen6") {

        core->startListening6(tokens[1].toInt());

    } else if (tokens[0] == "connect") {
        core->addSliverConnection(tokens[1], tokens[2].toInt());
        qDebug() << "Connecting to: " << tokens[1] << ":" << tokens[2];
    /*} else if (tokens[0] == "stop" && tokens[1] == "listening") {
        qDebug() << "stopping listening";
        serverList[tokens[2].toInt()]->close();*/
    } else if (tokens[0] == "disconnect") {
        qDebug() << "Removing socket";
        core->disconnectSocket(tokens[1].toInt());
    } else if (tokens[0] == "list") {
        QNetworkInterface interface;
        qDebug() <<interface.allAddresses();
        qDebug() << interface.allInterfaces();
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        int i = 0;
        foreach (QNetworkInterface interface, interfaces) {
            Interface interfaceStruct;
            qDebug() << QString("**Interface: %1 \n").arg(interface.humanReadableName());
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach (QNetworkAddressEntry entry, entries) {

                QHostAddress address = entry.ip();
                 qDebug() << (QString("%1: Address: %2 \n").arg(QString::number(i), address.toString()));
                 i++;
            }
        }

    } else if (tokens[0] == "ping") {
        int socketId = tokens[1].toInt();

        AbstractProtocol *protocol = core->getSocket(socketId)->getProtocol();
        protocol->sendPingRequest(tokens[2]);

    } else if (tokens[0] == "getinfo") {
        qDebug() << "Getting node info";
        int socketId = tokens[1].toInt();
        AbstractProtocol *protocol = core->getSocket(socketId)->getProtocol();
        protocol->sendNodeInfoRequest();
    } else if (tokens[0] == "exit_all") {
        core->shutDownNodes();
    }
}
