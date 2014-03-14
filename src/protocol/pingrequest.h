#ifndef PINGREQUEST_H
#define PINGREQUEST_H
#include "myqtcpsocket.h"
#include "abstractmessage.h"

struct PingRequestStruct
{
    QString ipv;
    QString remoteIp;
    QString localIp;
};

class PingRequest :public AbstractMessage
{
public:
    PingRequest();
    PingRequest(QString remoteIp);
    //void serialize(QByteArray *block);
    //void read(MyQTcpSocket *socket);
    PingRequestStruct data;

    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // PINGREQUEST_H
