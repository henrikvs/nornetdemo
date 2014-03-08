#include "nodeinfomessage.h"
#include "nodeprotocol.h"

QDataStream &operator<<(QDataStream &out, const Interface &interface)
{
    out << interface.interfaceName << interface.addresses;
    return out;
}

QDataStream &operator>>(QDataStream &in, Interface &interface)
{
    in >> interface.interfaceName >> interface.addresses;
    return in;
}

NodeInfoMessage::NodeInfoMessage(QObject *parent) : AbstractMessage(parent)
{
}

void NodeInfoMessage::serialize(QByteArray *block)
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.interfaces << data.listeningPort;
}

void NodeInfoMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.interfaces >> data.listeningPort;
}

char NodeInfoMessage::getType()
{
    return NodeProtocol::MSGTYPE_NODEINFO;
}
