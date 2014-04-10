#include "demostatusmessage.h"
#include <QDataStream>

DemoStatusMessage::DemoStatusMessage()
{

}

DemoStatusMessage::DemoStatusMessage(int version)
{
    this->data.version = version;
}

void DemoStatusMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.version;
}

void DemoStatusMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.version;
}

char DemoStatusMessage::getType() const
{
    return AbstractProtocol::MSGTYPE_DEMOSTATUS;
}
