#include "pingreply.h"
#include "nodeprotocol.h"
PingReply::PingReply()
{
}

PingReply::PingReply(QString ms, QString localIp, QString remoteHost, int sessionId, int state)
{
    data.ms = ms;
    data.localIp = localIp;
    data.remoteHost = remoteHost;
    data.sessionId = sessionId;
    data.state = state;
}

void PingReply::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.ms >> data.localIp >> data.remoteHost >> data.sessionId >> data.state;
}

void PingReply::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.ms << data.localIp << data.remoteHost << data.sessionId << data.state;
}

char PingReply::getType() const {
    return AbstractProtocol::MSGTYPE_PINGREPLY;
}
