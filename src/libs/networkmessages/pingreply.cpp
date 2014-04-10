#include "pingreply.h"
#include "nodeprotocol.h"
PingReply::PingReply()
{
}

PingReply::PingReply(QString ms)
{
    data.ms = ms;
}

void PingReply::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.ms;
}

void PingReply::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.ms;
}

char PingReply::getType() const {
    return AbstractProtocol::MSGTYPE_PINGREPLY;
}
