#include "nodeinfomessage.h"

QDataStream &operator<<(QDataStream &out, const NetworkInterface &interface)
{
    out << interface.interfaceName << interface.addresses;
    return out;
}

QDataStream &operator>>(QDataStream &in, NetworkInterface &interface)
{
    in >> interface.interfaceName >> interface.addresses;
    return in;
}

NodeInfoMessage::NodeInfoMessage()
{
}

void NodeInfoMessage::serialize(QByteArray *block)  const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.interfaces << data.listeningPort << data.lat << data.lng;
}

void NodeInfoMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.interfaces >> data.listeningPort >> data.lat >> data.lng;
}

char NodeInfoMessage::getType() const
{
    return MSGTYPE_NODEINFO;
}
