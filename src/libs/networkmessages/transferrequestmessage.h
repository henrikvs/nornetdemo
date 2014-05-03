#ifndef TRANSFERREQUESTMESSAGE_H
#define TRANSFERREQUESTMESSAGE_H
#include <QByteArray>
#include "abstractprotocol.h"

struct TransferRequestMessageStruct {
    int transferId;
    int transferType;
	QString remoteHost;
	QString localIp;
	int seconds;
};

class TransferRequestMessage : public AbstractMessage
{
public:
    static int nextId();
    static const int TRANSFER_TYPE_TCP = 1, TRANSFER_TYPE_UDP = 2, TRANSFER_TYPE_SCTP = 3, TRANSFER_TYPE_MPTCP = 4;
    TransferRequestMessage();
    TransferRequestMessage(int transferId, int transferType, QString remoteHost, QString localIp, int seconds);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    TransferRequestMessageStruct data;
};

#endif // TRANSFERREQUESTMESSAGE_H
