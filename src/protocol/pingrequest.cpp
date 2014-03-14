#include "pingrequest.h"
#include "nodeprotocol.h"
PingRequest::PingRequest()
{
}

PingRequest::PingRequest(QString remoteIp)
{
    data.remoteIp = remoteIp;
}

void PingRequest::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.ipv << data.localIp << data.remoteIp;
}

void PingRequest::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.ipv >> data.localIp >> data.remoteIp;

}

char PingRequest::getType() const {
    return NodeProtocol::MSGTYPE_PINGREQUEST;
}
