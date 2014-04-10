#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H
#include <QObject>
#include <QTcpSocket>
#include "abstractmessage.h"
#include "messageheader.h"
#include "pingreply.h"
#include "nodeinfomessage.h"
#include <QtEndian>

class MyQTcpSocket;

class AbstractProtocol : public QObject
{
    Q_OBJECT
public:
    static const int SIZE_MSGSIZE = (int)sizeof(quint16);
    static const int SIZE_MSGTYPE = (int)sizeof(quint8);
    static const int SIZE_HEADER = MessageHeader::SIZE;
    static const int MSGTYPE_PINGREQUEST = 1, MSGTYPE_PINGREPLY= 2,
                    MSGTYPE_COMMANDREQUEST=3, MSGTYPE_COMMANDREPLY = 4,
                    MSGTYPE_HEADER = 5, MSGTYPE_NODEINFO = 6, MSGTYPE_NODEINFOREQUEST= 7, MSGTYPE_DEMOSTATUS=8, MSGTYPE_NODESTATUSMESSAGE= 9, MSGTYPE_STRINGMESSAGE = 10;

    void sendPingRequest(QString remoteIp);
    void sendPingReply(QString ms);
    void sendNodeInfo(QStringList isps, QString port);
    void sendNodeInfoRequest();


    explicit AbstractProtocol(QObject *parent = 0);

    void setSocket(MyQTcpSocket *socket);
    void setType(qint32 type);
    virtual void start() {};
    qint32 getType();

protected:
    MyQTcpSocket *socket;
    virtual bool handleMessage(int type) = 0;

public slots:
    void newData();
    void sendMessage(const AbstractMessage &message);
private:
    MessageHeader header;
    bool headerRead;
    qint32 type;

};

#endif // ABSTRACTPROTOCOL_H
