#include "handshakeprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
#include <stdlib.h>
#include <QHostInfo>
#include <QTimer>
HandshakeProtocol::HandshakeProtocol(int connectionType, int entityType, QString expectedUsername, QString expectedHostname, QObject *parent) : AbstractProtocol(parent)
{
    this->connectionType = connectionType;
    this->expectedUsername = expectedUsername;
    this->expectedHostname = expectedHostname;
    this->entityType = entityType;
}


void HandshakeProtocol::start()
{
    QHostInfo info;
    qDebug() << "host info: " << info.localDomainName() << info.localHostName();
    QString hostName = info.localHostName();
#ifdef Q_OS_UNIX
    QString sliceName = getenv("USER");
    if (sliceName == "root") {
        sliceName = getenv("SUDO_USER");
    }
#elif defined(Q_OS_WIN)
    qDebug() << "user" << getenv("USERNAME");
    QString sliceName = getenv("USERNAME");
#endif
    HandshakeMessage message(sliceName, hostName, expectedUsername, expectedHostname, connectionType, entityType);
    sendMessage(message);
    qDebug() << "Starting handshake protocol, entity type: " << message.data.entityType << "username: " << message.data.username << "hostname:"
             << message.data.hostname << "Expected username:" << message.data.expectedUsername << "Expected hostname:" << message.data.expectedHostname;
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    timer->start(10000);*/

}



void HandshakeProtocol::cleanUp()
{

}

bool HandshakeProtocol::finishedReading()
{
    return true;
}


bool HandshakeProtocol::handleMessage(int type)
{
    if (type == AbstractMessage::MSGTYPE_HANDSHAKE) {
        HandshakeMessage message;
        if (message.data.connectionType == NetworkEntity::CONNECTION_TYPE_REPLY) { //TODO explain this please
            message.data.connectionType = connectionType;
        }
        message.read(socket);
        active = false;
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(newData()));
        emit newHandShake(message, socket);
        deleteLater();
    }
    return true;
}
