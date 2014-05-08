#include "networklibglobal.h"

#ifndef PINGREQUEST_H
#define PINGREQUEST_H
#include "myqtcpsocket.h"
#include "abstractmessage.h"

struct PingRequestStruct
{
    int sessionId;
    QString seconds;
    QString interval;
    QString ipv;
    QString remoteIp;
    QString localIp;
};

class NETWORKLIB_EXPORT PingRequest :public AbstractMessage
{
public:
    PingRequest();
    PingRequest(int sessionId, QString remoteIp, QString localIp);
    //void serialize(QByteArray *block);
    //void read(MyQTcpSocket *socket);
    PingRequestStruct data;

    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // PINGREQUEST_H
