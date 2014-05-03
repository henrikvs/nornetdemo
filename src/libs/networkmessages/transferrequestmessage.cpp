#include "transferrequestmessage.h"
#include <QDataStream>

int TransferRequestMessage::nextId()
{
    static int id=0;
    return id++;
}

TransferRequestMessage::TransferRequestMessage()
{

}

TransferRequestMessage::TransferRequestMessage(int transferId, int transferType, QString remoteHost, QString localIp, int seconds)
{
    this->data.transferId = transferId;
	this->data.transferType=transferType;
	this->data.remoteHost=remoteHost;
	this->data.localIp=localIp;
	this->data.seconds=seconds;
}

void TransferRequestMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.transferId << data.transferType << data.remoteHost << data.localIp << data.seconds;
}

void TransferRequestMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.transferId >> data.transferType >> data.remoteHost >> data.localIp >> data.seconds;
}

char TransferRequestMessage::getType() const
{
    return AbstractProtocol::MSGTYPE_TRANSFERREQUEST;
}

