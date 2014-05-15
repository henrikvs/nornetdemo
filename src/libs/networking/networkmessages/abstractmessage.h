#include "networklibglobal.h"

#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include <QByteArray>
#include "myqtcpsocket.h"
class NETWORKLIB_EXPORT AbstractMessage
{
public:
    AbstractMessage();

    static const int MSGTYPE_PINGREQUEST = 1;
    static const int MSGTYPE_PINGREPLY= 2;
    static const int MSGTYPE_COMMANDREQUEST=3;
    static const int MSGTYPE_COMMANDREPLY = 4;
    static const int MSGTYPE_HEADER = 5;
    static const int MSGTYPE_NODEINFO = 6;
    static const int MSGTYPE_NODEINFOREQUEST= 7;
    static const int MSGTYPE_DEMOSTATUS=8;
    static const int MSGTYPE_NODESTATUSMESSAGE= 9;
    static const int MSGTYPE_STRINGMESSAGE = 10;
    static const int MSGTYPE_HANDSHAKE = 11;
    static const int MSGTYPE_TRANSFERREQUEST= 12;
    static const int MSGTYPE_TRANSFERSTATUS = 13;
    static const int MSGTYPE_TASKCONTROLLER= 14;
    virtual void serialize(QByteArray *block) const;
    virtual void read(MyQTcpSocket *socket) ;
    virtual char getType() const;
};

#endif // ABSTRACTMESSAGE_H
