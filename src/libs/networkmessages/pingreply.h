#ifndef PINGREPLY_H
#define PINGREPLY_H
#include <QString>
#include "myqtcpsocket.h"
#include "abstractmessage.h"
struct PingReplyStruct
{
    QString ms;
    QString localIp;
    QString remoteHost;
    int sessionId;
};

class PingReply : public AbstractMessage
{
public:
    PingReply();
    PingReply(QString ms, QString localIp, QString remoteHost, int sessionId);
    //void read(MyQTcpSocket *socket);
    //void serialize(QByteArray *block);
    PingReplyStruct data;

    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // PINGREPLY_H
