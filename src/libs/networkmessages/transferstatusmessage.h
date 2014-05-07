#ifndef TRANSFERSTATUSMESSAGE_H
#define TRANSFERSTATUSMESSAGE_H
#include <QByteArray>
#include "abstractprotocol.h"

struct TransferStatusMessageStruct {
	int transferId;
	QString mbps;
	QString remoteHost;
	QString localIp;
	int interval;
	int state;
};

class TransferStatusMessage : public AbstractMessage
{
public:
    static const int STATE_RUNNING = 1;
    static const int STATE_FINISHED = 2;
    TransferStatusMessage();
    TransferStatusMessage(int transferId, QString mbps, QString remoteHost, QString localIp, int interval, int state);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    TransferStatusMessageStruct data;
};

#endif // TRANSFERSTATUSMESSAGE_H
