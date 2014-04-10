#ifndef NETWORKENTITY_H
#define NETWORKENTITY_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QList>
#include "myqtcpsocket.h"
#include "myqtcpserver.h"
#include "flexiserver.h"
#include "pingreply.h"
#include "nodeinfomessage.h"
#include "abstractprotocol.h"

class AbstractProtocol;
class NetworkEntity : public QObject
{
    Q_OBJECT
public:
    static const int CONNECTION_TYPE_DEMO = 1, CONNECTION_TYPE_NODE = 2;
    static const int VERSION = 19;
    static const int EXIT_TYPE_NORMAL = 1;
    static const int EXIT_TYPE_UPDATE = 2;
    static const int EXIT_TYPE_RESTART = 3;
    static const int EXIT_TYPE_ERROR = 4;
    explicit NetworkEntity(QObject *parent = 0);
    ~NetworkEntity();
    MyQTcpSocket *clientSocket;
    void addConnection(QString ip, int port, int type=0, QString name = QString());
    void connectToNode(QString ip, int port);
    void makeConnection(QString ip, int port);
    void startListening(int port);
    void startListening(int port, int interface);
    void startListening6(int port);
    void stopListening();
    void removeSocket(int socketId);
    void disconnectSocket(int socketId);
    MyQTcpSocket* getSocket(int id);
    QHash<int, MyQTcpSocket*> socketHash;
protected:
    virtual AbstractProtocol *createProtocol(int type, MyQTcpSocket *socket) = 0;
    int getNextId();
private:
    void connectToFrontEnd(QString ip, int port);
    QList<MyQTcpServer*> serverList;

    int nextId;
    void sendProtocolType(MyQTcpSocket *socket, qint32 type);

signals:
    //void newNode_signal(Node node, int node);
    //void newPing_signal(Node node, ms);
    //void newReport_signal(Node node);
    void connectionError(QString hostName, int port);
    void shutDownComplete(int exitValue);

public slots:
    virtual void connected();
    virtual void disconnected();
    void newConnection(qintptr socketDescriptor);
    //void newNodeData();
    //void newFrontEndData();
    void newData();
    virtual void connectionError(QAbstractSocket::SocketError error);
    virtual bool newStdIn(QString input);
    void shutDown(int exitValue = EXIT_TYPE_NORMAL);
};

#endif // NETWORKENTITY_H
