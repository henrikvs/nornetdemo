#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "myqtcpsocket.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    MyQTcpSocket *clientSocket;
private:
    void addConnection(QString ip, int port);
    void connectToNode(QString ip, int port);
    void startListening(int port);

signals:

public slots:
    void connected();
    void newConnection();
    void newNodeData();
    void newFrontEndData();
    void newData();
    void error(QAbstractSocket::SocketError error);

};

#endif // CLIENT_H
