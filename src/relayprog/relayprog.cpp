#include <QCoreApplication>
#include "relayprog.h"

RelayProg::RelayProg()
{
    connect(this, SIGNAL(shutDownComplete(int)), this, SLOT(exitProgram(int)));

}

/*
 *  We override the handshake protocol of the abstract class, and circumvent the handshake all together
 */
void RelayProg::startHandshakeProtocol(int connectionType, QString name, QString remoteName, MyQTcpSocket *socket)
{
    qDebug() << "Starting relay protocol";
    RelayProtocol *protocol = new RelayProtocol(this);
    protocol->setSocket(socket);
    socket->setProtocol(protocol);
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    connect(protocol, SIGNAL(newConnection(HandshakeMessage,RelayProtocol*)), this, SLOT(handleNewConnection(HandshakeMessage,RelayProtocol*)));
    if (socket->bytesAvailable()) { //since we made the connection quite late, lets make sure there's no pending data
        QMetaObject::invokeMethod(protocol, "newData", Qt::QueuedConnection);
    }

}

void RelayProg::disconnected(MyQTcpSocket* socket)
{
    qDebug() << "Disconnecting";
    RelayProtocol *protocol = (RelayProtocol*)socket->getProtocol();
    protocol->cleanUp();
    connected.removeAll(protocol->getHandshakeMessage().data.hostname);
    {
    QHashIterator<QString, RelayProtocol*> i(pendingNodes);
    QString remove;
    while (i.hasNext())  {
        qDebug() << "checking to remove";
        i.next();
        if (i.value() == protocol) {
            remove = i.key();
            qDebug() << "Removed key";
        }
    }
    if (!remove.isEmpty()) {
        pendingNodes.remove(remove);
        return;
    }


    }
    {
    QHashIterator<QString, RelayProtocol*> i(pendingDemos);
    QString remove;
    while (i.hasNext())  {
        i.next();
        if (i.value() == protocol) {
            remove = i.key();
            qDebug() << "Removed key";
        }
    }
    if (!remove.isEmpty()) {
        pendingDemos.remove(remove);
    }

    }

}

void RelayProg::newKeyboardInput(QString text)
{
    if (text == "list") {
        qDebug() << "Pending nodes: " << pendingNodes.keys();
        qDebug() << "Pending GUI: " << pendingDemos.keys();
        qDebug() << " Connected: " << connected;
    }
}


int RelayProg::getEntityType()
{
    return 0;
}

AbstractProtocol *RelayProg::createProtocol(HandshakeMessage message, MyQTcpSocket *socket)
{
    return NULL;
}


void RelayProg::exitProgram(int exitValue)
{
    qApp->exit(exitValue);
}

void RelayProg::handleNewConnection(HandshakeMessage message, RelayProtocol *protocol)
{
     qDebug() << "Received new connect: entity type: " << message.data.entityType << "name: " << message.data.username << message.data.hostname
                 << message.data.expectedUsername << message.data.expectedHostname;
    if (message.data.entityType == ENTITY_TYPE_DEMO) {
        qDebug() << "Received connect from demo";
        QString id = message.data.expectedUsername + "@" + message.data.expectedHostname;
        if (pendingNodes.contains(id)) {
            RelayProtocol *remoteProtocol = pendingNodes[id];
            pendingNodes.remove(id);
            protocol->setRemoteProtocol(remoteProtocol);
            remoteProtocol->setRemoteProtocol(protocol);
            connected << message.data.hostname << remoteProtocol->getHandshakeMessage().data.hostname;
        } else {
            pendingDemos[id]= protocol;
        }
    } else if (message.data.entityType == ENTITY_TYPE_NODE) {
        qDebug() << "Received connect from node";
        QString id = message.data.username + "@" + message.data.hostname;
        if (pendingDemos.contains(id)) {
            RelayProtocol *remoteProtocol = pendingDemos[id];
            pendingDemos.remove(id);
            protocol->setRemoteProtocol(remoteProtocol);
            remoteProtocol->setRemoteProtocol(protocol);
            connected << message.data.hostname << remoteProtocol->getHandshakeMessage().data.hostname;
        } else {
            pendingNodes[id]= protocol;
        }
    }

}
