#include "pingrequest.h"
#include "nodeprotocol.h"
PingRequest::PingRequest()
{
}

PingRequest::PingRequest(int sessionId, QString remoteIp, QString localIp)
{
    data.sessionId = sessionId;
    data.remoteIp = remoteIp;
    data.localIp = localIp;
}

void PingRequest::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.interval << data.seconds << data.sessionId << data.ipv << data.localIp << data.remoteIp;
}

void PingRequest::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.interval >> data.seconds >> data.sessionId >> data.ipv >> data.localIp >> data.remoteIp;

}

char PingRequest::getType() const {
    return AbstractProtocol::MSGTYPE_PINGREQUEST;
}
