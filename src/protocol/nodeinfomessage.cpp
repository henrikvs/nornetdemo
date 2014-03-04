#include "nodeinfomessage.h"
#include "nodeprotocol.h"

NodeInfoMessage::NodeInfoMessage()
{
}

void NodeInfoMessage::serialize(QByteArray *block)
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.ISPConnections << data.listeningPort;
}

void NodeInfoMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.ISPConnections >> data.listeningPort;
}

char NodeInfoMessage::getType()
{
    return NodeProtocol::MSGTYPE_NODEINFO;
}
