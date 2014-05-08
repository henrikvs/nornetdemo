#include "networklibglobal.h"

#ifndef TRANSFERREQUESTMESSAGE_H
#define TRANSFERREQUESTMESSAGE_H
#include <QByteArray>
#include "abstractmessage.h"

struct TransferRequestMessageStruct {
    int transferId;
    int transferType;
	QString remoteHost;
	QString localIp;
	int seconds;
};

class NETWORKLIB_EXPORT TransferRequestMessage : public AbstractMessage
{
public:
    static int nextId();
    static const int TRANSFER_TYPE_TCP = 1;
    static const int TRANSFER_TYPE_UDP = 2;
    static const int TRANSFER_TYPE_SCTP = 3;
    static const int TRANSFER_TYPE_MPTCP = 4;
    TransferRequestMessage();
    TransferRequestMessage(int transferId, int transferType, QString remoteHost, QString localIp, int seconds);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    TransferRequestMessageStruct data;
};

#endif // TRANSFERREQUESTMESSAGE_H
