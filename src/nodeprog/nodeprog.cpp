#include <QCoreApplication>
#include "nodeprog.h"

NodeProg::NodeProg()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));
}

AbstractProtocol *NodeProg::createProtocol(int type, MyQTcpSocket *socket)
{
    if (type == CONNECTION_TYPE_DEMO) {
        NodeProtocol *protocol = new NodeProtocol(getNextId(), this);
        protocol->setSocket(socket);
        connect(protocol, SIGNAL(exitProgram(int)), this, SLOT(shutDown(int)));
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << type << "Should be " << CONNECTION_TYPE_DEMO;
        return NULL;
    }
}

void NodeProg::failedToConnect(QString host, int port)
{
    qDebug() << "failed to connect to: " << host << port;
}

void NodeProg::connected(QString host, int port)
{
    qDebug() << "Connection success: " << host << port;
}

void NodeProg::exitProgram(int exitValue)
{
    qApp->exit(exitValue);
}
