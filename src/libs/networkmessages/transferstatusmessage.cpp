#include "transferstatusmessage.h"
#include <QDataStream>

TransferStatusMessage::TransferStatusMessage()
{

}

TransferStatusMessage::TransferStatusMessage(int transferId, QString mbps, QString remoteHost, QString localIp, int interval, int state)
{
	this->data.transferId=transferId;
	this->data.mbps=mbps;
	this->data.remoteHost=remoteHost;
	this->data.localIp=localIp;
	this->data.interval=interval;
	this->data.state=state;
}

void TransferStatusMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.transferId << data.mbps << data.remoteHost << data.localIp << data.interval << data.state;
}

void TransferStatusMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.transferId >> data.mbps >> data.remoteHost >> data.localIp >> data.interval >> data.state;
}

char TransferStatusMessage::getType() const
{
    return AbstractProtocol::MSGTYPE_TRANSFERSTATUS;
}

