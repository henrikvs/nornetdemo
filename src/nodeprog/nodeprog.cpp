#include <QCoreApplication>
#include "nodeprog.h"

NodeProg::NodeProg()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));
}

void NodeProg::start(int port)
{
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
    /*if (socket->isRelay) {
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, getName());
    }*/
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
    qApp->exit(exitValue);
}

int NodeProg::getEntityType()
{
    return NetworkEntity::ENTITY_TYPE_NODE;
}
