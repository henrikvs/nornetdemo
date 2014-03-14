#ifndef NODEPROTOCOL_H
#define NODEPROTOCOL_H
#include <QObject>
#include <QTcpSocket>
#include "myqtcpsocket.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "pingreply.h"
#include "nodeinfomessage.h"
#include <QtEndian>

class NodeProtocol : public QObject
{
    Q_OBJECT
public:
    static const int SIZE_MSGSIZE = (int)sizeof(quint16);
    static const int SIZE_MSGTYPE = (int)sizeof(quint8);
    static const int SIZE_HEADER = MessageHeader::SIZE;
    static const int MSGTYPE_PINGREQUEST = 1, MSGTYPE_PINGREPLY= 2,
                    MSGTYPE_COMMANDREQUEST=3, MSGTYPE_COMMANDREPLY = 4,
                    MSGTYPE_HEADER = 5, MSGTYPE_NODEINFO = 6, MSGTYPE_NODEINFOREQUEST= 7;
    void sendPingRequest(QString remoteIp);
    void sendPingReply(QString ms);
    void sendNodeInfo(QStringList isps, QString port);
    void sendNodeInfoRequest();


    explicit NodeProtocol(MyQTcpSocket *socket, int id, QObject *parent = 0);

    int getId();

    void setHostName(QString name);
    void setPort(int port);

    QString getHostName();
    int getPort();
signals:
    void newPingReply(PingReply message, int id);
    void newNodeInfo(NodeInfoMessage message, int id);
public slots:
    void newData();
    void sendMessage(const AbstractMessage &message);
private:
    MyQTcpSocket *socket;
    MessageHeader header;
    bool headerRead;
    int id;
    QString hostName;
    int port;

};

#endif // CLIENTPROTOCOL_H
