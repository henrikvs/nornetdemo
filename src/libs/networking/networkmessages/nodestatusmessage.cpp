#include "nodestatusmessage.h"
#include <QDataStream>
NodeStatusMessage::NodeStatusMessage()
{
}

NodeStatusMessage::NodeStatusMessage(int status)
{
    data.status = status;
}

void NodeStatusMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.status;
}

void NodeStatusMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.status;
}

char NodeStatusMessage::getType() const
{
    return MSGTYPE_NODESTATUSMESSAGE;
}
