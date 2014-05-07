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
    int state;
};

class PingReply : public AbstractMessage
{
public:
    static const int STATE_RUNNING = 1;
    static const int STATE_FINISHED = 2;
    PingReply();
    PingReply(QString ms, QString localIp, QString remoteHost, int sessionId, int state);
    //void read(MyQTcpSocket *socket);
    //void serialize(QByteArray *block);
    PingReplyStruct data;

    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // PINGREPLY_H
