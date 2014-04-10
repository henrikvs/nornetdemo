#include <QCoreApplication>
#include "relayprog.h"

RelayProg::RelayProg()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));
}

AbstractProtocol *RelayProg::createProtocol(int type, MyQTcpSocket *socket)
{
    if (type == CONNECTION_TYPE_DEMO) {
        RelayProtocol *protocol = new RelayProtocol(this);
        protocol->setSocket(socket);
        connect(protocol, SIGNAL(exitProgram(int)), this, SLOT(shutDown(int)));
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << type << "Should be " << CONNECTION_TYPE_DEMO;
        return NULL;
    }
}

void RelayProg::failedToConnect(QString host, int port)
{
    qDebug() << "failed to connect to: " << host << port;
}

void RelayProg::connected(QString host, int port)
{
    qDebug() << "Connection success: " << host << port;
}

void RelayProg::exitProgram(int exitValue)
{
    qApp->exit(exitValue);
}
