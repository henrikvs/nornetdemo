#include <QCoreApplication>
#include "nodeprog.h"
#include <QProcess>
#include <QStringList>

NodeProg::NodeProg() : NetworkEntity()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));
}

void NodeProg::start(int port)
{
    startIperf(getIperf4Port());
    startIperfIpv6(getIperf6Port());
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
        NodeProtocol *protocol = new NodeProtocol(getIperf4Port(), getIperf6Port(), this);
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
    if (iperf4Process != NULL) {
        iperf4Process->terminate();
        iperf4Process->waitForFinished();
        qDebug() << "Iperf4 finished";
        iperf4Process = NULL; //should be automatically freed by the child parent relationship
    }

    if (iperf6Process != NULL) {
        iperf6Process->terminate();
        iperf6Process->waitForFinished();
        qDebug() << "Iperf6 finished";
        iperf6Process = NULL; //should be automatically freed by the child parent relationship
    }
    qApp->exit(exitValue);
}

int NodeProg::getEntityType()
{
    return NetworkEntity::ENTITY_TYPE_NODE;
}

void NodeProg::startIperf(int port)
{
    qDebug() << "Starting iperf on port " << port;
    iperf4Process = new QProcess(this);

    iperf4Process->start("iperf", QStringList() << "-p" << QString::number(port) << "-s");

    //connect(iperfProces, &QProcess::)
    void (QProcess:: *signal)(int) = &QProcess::finished;
    connect(iperf4Process, signal, [](int val) {
        qDebug() << "Destroyed";
    });

}

void NodeProg::startIperfIpv6(int port)
{
    qDebug() << "Starting iperf on port" << port;
    iperf6Process = new QProcess(this);

    iperf6Process->start("iperf", QStringList() << "-V" << "-p" << QString::number(port) << "-s");

    //connect(iperfProces, &QProcess::)
    void (QProcess:: *signal)(int) = &QProcess::finished;
    connect(iperf6Process, signal, [](int val) {
        qDebug() << "Destroyed";
    });
}
