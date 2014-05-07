#include <QCoreApplication>
#include "nodeprog.h"
#include <QProcess>
#include <QStringList>

NodeProg::NodeProg()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));
    iperfProcess = NULL;
}

void NodeProg::start(int port)
{
    startIperf();
    if (relayEnabled()) {
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, "any", "any");
    } else {
        startListening6(port);
        startListening(port);
    }
}

void NodeProg::disconnected(MyQTcpSocket *socket)
{
    qDebug() << "Disconnecting nodeprog";
    if (socket->isRelay) { //if this connection is a relay connection, we reconnect when disconnected, to always make this node available on the relayserver
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, "any", "any");
    }
}

AbstractProtocol *NodeProg::createProtocol(HandshakeMessage message, MyQTcpSocket *socket)
{
    if (message.data.connectionType == CONNECTION_TYPE_DEMO || message.data.connectionType == CONNECTION_TYPE_RELAY) {
        NodeProtocol *protocol = new NodeProtocol(this);
        protocol->setSocket(socket);
        connect(protocol, SIGNAL(exitProgram(int)), this, SLOT(shutDown(int)));
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << message.data.connectionType << "Should be " << CONNECTION_TYPE_DEMO;
        return NULL;
    }
}

void NodeProg::exitProgram(int exitValue)
{
    qDebug() << "Exiting nodeprog";
    if (iperfProcess != NULL) {
        iperfProcess->terminate();
        iperfProcess->waitForFinished();
        iperfProcess = NULL; //should be automatically freed by the child parent relationship
    }
    qApp->exit(exitValue);
}

int NodeProg::getEntityType()
{
    return NetworkEntity::ENTITY_TYPE_NODE;
}

void NodeProg::startIperf()
{
    qDebug() << "Starting iperf";
    QProcess *iperfProcess = new QProcess(this);

    iperfProcess->start("iperf", QStringList() << "-s");

    //connect(iperfProces, &QProcess::)
    void (QProcess:: *signal)(int) = &QProcess::finished;
    connect(iperfProcess, signal, [](int val) {
        qDebug() << "Destroyed";
    });

}
