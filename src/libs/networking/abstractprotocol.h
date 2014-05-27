#include "networklibglobal.h"

#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H
#include <QObject>
#include <QTcpSocket>
#include "abstractmessage.h"
#include "messageheader.h"

class MyQTcpSocket;
/**
 * @brief The AbstractProtocol class is a base class for the various protocols such as RelayProtocol, NodeProtocol and DemoProtocol
 */
class NETWORKLIB_EXPORT AbstractProtocol : public QObject
{
    Q_OBJECT
public:
    static const int SIZE_MSGSIZE = (int)sizeof(quint16);
    static const int SIZE_MSGTYPE = (int)sizeof(quint8);
    static const int SIZE_HEADER = MessageHeader::SIZE;
//static const int MSGTYPE_PINGREQUEST = 1, MSGTYPE_PINGREPLY= 2,
  //                  MSGTYPE_COMMANDREQUEST=3, MSGTYPE_COMMANDREPLY = 4,
    //                MSGTYPE_HEADER = 5, MSGTYPE_NODEINFO = 6, MSGTYPE_NODEINFOREQUEST= 7,
      //              MSGTYPE_DEMOSTATUS=8, MSGTYPE_NODESTATUSMESSAGE= 9, MSGTYPE_STRINGMESSAGE = 10,
        //            MSGTYPE_HANDSHAKE = 11, MSGTYPE_TRANSFERREQUEST= 12, MSGTYPE_TRANSFERSTATUS = 13;

    //void sendPingReply(QString ms);


    explicit AbstractProtocol(QObject *parent = 0);

    void setSocket(MyQTcpSocket *socket);
    void setType(qint32 type);
    virtual void cleanUp() = 0;
    virtual void start() {};
    qint32 getType();
    bool isRelay;
    void setName(QString name);
    QString getName();

protected:
    MessageHeader header;
    MyQTcpSocket *socket;
    virtual bool handleMessage(int type) = 0;

public slots:
    void newData();
    void sendMessage(const AbstractMessage &message);
private:
    bool headerRead;
    qint32 type;
    QString name;
    bool active;
    void sendHeartbeat();

};

#endif // ABSTRACTPROTOCOL_H
