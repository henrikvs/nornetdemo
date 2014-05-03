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
#include "handshakeprotocol.h"

class AbstractProtocol;
class NetworkEntity : public QObject
{
    Q_OBJECT
public:
    static const int CONNECTION_TYPE_DEMO = 1, CONNECTION_TYPE_NODE = 2, CONNECTION_TYPE_RELAY = 3, CONNECTION_TYPE_REPLY = 4;
    static const int ENTITY_TYPE_DEMO = 1, ENTITY_TYPE_NODE = 2;
    static const int VERSION = 19;
    static const int EXIT_TYPE_NORMAL = 1;
    static const int EXIT_TYPE_UPDATE = 2;
    static const int EXIT_TYPE_RESTART = 3;
    static const int EXIT_TYPE_ERROR = 4;
    explicit NetworkEntity(QObject *parent = 0);
    ~NetworkEntity();
    MyQTcpSocket *clientSocket;
    void addConnection(QString ip, int port, int type, QString username, QString hostname);
    //void addRelayConnection(QString ip, int port, int type=0, QString name = QString());
    void startListening(int port);
    void startListening(int port, int interface);
    void startListening6(int port);
    void stopListening();
    void removeSocket(int socketId);
    void disconnectSocket(int socketId);
    void enableRelay(QString address, int port);
    MyQTcpSocket* getSocket(int id);
    QHash<int, MyQTcpSocket*> socketHash;
    virtual void disconnected(MyQTcpSocket *socket) = 0;
    void setName(QString name);
    QString getName();

    virtual void startHandshakeProtocol(int connectionType, QString expectedUsername, QString expectedHostname, MyQTcpSocket *socket);
protected:
    virtual AbstractProtocol *createProtocol(HandshakeMessage message, MyQTcpSocket *socket) = 0;
    int getNextId();
    int nextRequestId();
    //relay
    bool relayEnabled();
    QString getRelayAddress();
    int getRelayPort();

private:
    QList<MyQTcpServer*> serverList;

    int nextId;
    void sendProtocolType(MyQTcpSocket *socket, qint32 type);
    //relay
    bool relayOn;
    QString relayAddress;
    int relayPort;
    QString name;
    void setUpSocket(MyQTcpSocket *socket);
    virtual int getEntityType() = 0;

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
    virtual void connectionError(QAbstractSocket::SocketError error);
    virtual bool newStdIn(QString input);
    void shutDown(int exitValue = EXIT_TYPE_NORMAL);
private slots:
    virtual void handleNewHandshake(HandshakeMessage message, MyQTcpSocket *socket);
};

#endif // NETWORKENTITY_H
