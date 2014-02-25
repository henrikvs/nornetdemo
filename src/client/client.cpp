#include "client.h"
#include <QTcpServer>
Client::Client(QObject *parent) :
    QObject(parent)
{
    connectToFrontEnd("127.0.0.1", 3333);
}

void Client::addConnection(QString ip, int port)
{
    MyQTcpSocket *socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(newClientData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
}

void Client::connectToNode(QString ip, int port)
{
    addConnection(ip, port);
}

void Client::connectToFrontEnd(QString ip, int port)
{
    addConnection(ip, port);
}

void Client::startListening(int port)
{
    MyQTcpServer *server = new MyQTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
    server->listen(QHostAddress::Any, 3333);
}

void Client::connected()
{
    qDebug() << "connected";
}

void Client::newConnection()
{
    MyQTcpServer *server = (QTcpServer*) sender();
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(), this, SLOT(newData()));
}

void Client::newNodeData()
{
    QTcpSocket *socket = (QTcpSocket*) sender();
    socket->pro
    socket->write("success");
    socket->bytesAvailable();

}

void Client::newFrontEndData()
{

}


void Client::error(QAbstractSocket::SocketError error)
{
    qDebug() << "connection problem";
}
