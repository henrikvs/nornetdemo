#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QList>
#include "myqtcpsocket.h"
#include "myqtcpserver.h"
#include "flexiserver.h"
#include "nodeprotocol.h"
#include "pingreply.h"
#include "nodeinfomessage.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    MyQTcpSocket *clientSocket;
    void addConnection(QString ip, int port);
    void connectToNode(QString ip, int port);
    void startListening(int port);
    void startListening(int port, int interface);
    void startListening6(int port);
    void stopListening();
private:
    void connectToFrontEnd(QString ip, int port);
    QHash<int, MyQTcpSocket*> socketHash;
    MyQTcpServer *server;
    int nextId;
    int getNextId();

signals:
    //void newNode_signal(Node node, int node);
    //void newPing_signal(Node node, ms);
    //void newReport_signal(Node node);
    void newPingReply_signal(PingReply message, int id);
    void newNodeInfo_signal(NodeInfoMessage message, int id);
    void connectionError(QString hostName, int port);

public slots:
    void connected();
    void newConnection(qintptr socketDescriptor);
    //void newNodeData();
    //void newFrontEndData();
    void newData();
    void error(QAbstractSocket::SocketError error);
    void newStdIn(QString input);
    void shutDown();
    void newPingReply_slot(PingReply message, int id);
    void newNodeInfo_slot(NodeInfoMessage message, int id);

};

#endif // CLIENT_H
