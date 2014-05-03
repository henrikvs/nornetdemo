#include "handshakemessage.h"
#include <QDataStream>

HandshakeMessage::HandshakeMessage()
{

}

HandshakeMessage::HandshakeMessage(QString username, QString hostname, QString expectedUsername, QString expectedHostname, int connectionType, int entityType)
{
    this->data.username = username;
    this->data.hostname = hostname;
    this->data.expectedUsername = expectedUsername;
    this->data.expectedHostname = expectedHostname;
    this->data.connectionType = connectionType;
    this->data.entityType = entityType;
}

void HandshakeMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.username << data.hostname << data.expectedUsername << data.expectedHostname << data.connectionType << data.entityType;
}

void HandshakeMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.username >> data.hostname >> data.expectedUsername >> data.expectedHostname >> data.connectionType >> data.entityType;
}

char HandshakeMessage::getType() const
{
    return AbstractProtocol::MSGTYPE_HANDSHAKE;
}
