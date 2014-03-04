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
    QList<MyQTcpSocket*> socketList;
    MyQTcpServer *server;
    NodeProtocol *protocol;

signals:

public slots:
    void connected();
    void newConnection(qintptr socketDescriptor);
    //void newNodeData();
    //void newFrontEndData();
    void newData();
    void error(QAbstractSocket::SocketError error);
    void newStdIn(QString input);
    void shutDown();

};

#endif // CLIENT_H
