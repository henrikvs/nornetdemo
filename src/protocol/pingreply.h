#ifndef PINGREPLY_H
#define PINGREPLY_H
#include <QString>
#include "myqtcpsocket.h"
#include "abstractmessage.h"
struct PingReplyStruct
{
    QString ms;
};

class PingReply : public AbstractMessage
{
public:
    PingReply();
    PingReply(QString ms);
    //void read(MyQTcpSocket *socket);
    //void serialize(QByteArray *block);
    PingReplyStruct data;

    void serialize(QByteArray *block);
    void read(MyQTcpSocket *socket);
    char getType();
};

#endif // PINGREPLY_H
