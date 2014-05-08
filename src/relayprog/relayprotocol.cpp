#include "relayprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
#include <QTimer>
RelayProtocol::RelayProtocol(QObject *parent) : AbstractProtocol(parent)
{
    remoteProtocol = NULL;

}


void RelayProtocol::start()
{
    qDebug() << "Starting protocol";
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    timer->start(10000);*/
}

void RelayProtocol::setRemoteProtocol(RelayProtocol *protocol)
{
    qDebug() << "Setting remote protocol";
    remoteProtocol = protocol;
    sendData(buffer, remoteProtocol->socket);
    buffer.clear();
}

void RelayProtocol::cleanUp()
{
    qDebug() << "Cleaning up";
    if (remoteProtocol != NULL) {
        remoteProtocol->remoteDisconnected();
    }
    remoteProtocol = NULL;
}

void RelayProtocol::sendData(QByteArray &buffer, MyQTcpSocket *sock)
{
    qDebug() << "Writing data size: " << buffer.size();

    sock->write(buffer);
}

void RelayProtocol::remoteDisconnected()
{
    socket->disconnectFromHost();
}



bool RelayProtocol::handleMessage(int type)
{  
    int dataSize = header.data.size;
    qDebug() << "datasize:" << dataSize;
    header.serialize(&buffer);
    qDebug() << "Handling message relay";
    if (type == AbstractMessage::MSGTYPE_HANDSHAKE) {
        HandshakeMessage message;
        message.read(socket);
        qDebug() << "New handshake message:" << message.data.connectionType << message.data.entityType << message.data.username << message.data.hostname
                    << message.data.expectedUsername << message.data.expectedHostname;
        message.serialize(&buffer);
        emit newConnection(message, this);
    } else {
        buffer.append(socket->read(dataSize));
    }

    if (remoteProtocol != NULL) {
        sendData(buffer, remoteProtocol->socket);
        buffer.clear();
    }
    return true;
}
