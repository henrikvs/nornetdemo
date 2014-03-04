#include "client.h"
#include "myqtcpserver.h"
#include <QNetworkInterface>
#include <QCoreApplication>
Client::Client(QObject *parent) :
    QObject(parent)
{
    //connectToFrontEnd("127.0.0.1", 3333);
    //qRegisterMetaType<qintptr>("qintptr");
    server = new MyQTcpServer(this);
}

Client::~Client()
{
    qDebug() << "Destructing";
    foreach (MyQTcpSocket *socket, socketList) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
        socket->close();
        qDebug() << "Disconnected";
    }
}

void Client::addConnection(QString ip, int port)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    protocol = new NodeProtocol(socket, this);
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
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
    //QThread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(QHostAddress::AnyIPv4, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() ;
    //thread->start();
    //server->listen(QHostAddress::Any, port);
}

void Client::startListening6(int port)
{
    //QThread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(QHostAddress::AnyIPv6, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() ;
    //thread->start();
    //server->listen(QHostAddress::Any, port);
}

void Client::startListening(int port, int interface)
{
    QNetworkInterface inter;
    //Thread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(inter.allAddresses()[interface], port);
    //thread->start();
    //server->listen(QHostAddress::Any, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() ;
}

void Client::stopListening()
{
    qDebug() << "Stopping server";
    //server->stopListening();
}

void Client::connected()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    socketList << socket;

    qDebug() << "connected";
}

void Client::newConnection(qintptr socketDescriptor)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    protocol = new NodeProtocol(socket, this);
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    socket->setSocketDescriptor(socketDescriptor);
    socketList << socket;
    qDebug() << "New connection: " << socket->peerAddress().toString();
}

/*
void Client::newNodeData()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    socket->
    socket->pro
    socket->write("success");
    socket->bytesAvailable();

}

void Client::newFrontEndData()
{

}
*/
void Client::newData()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    qDebug() << socket->readAll();
}

void Client::newStdIn(QString input)
{
    QStringList tokens = input.split(QRegExp("\\s"));
    if (tokens[0] == "listen") {
        if (tokens.length() == 3) {
            startListening(tokens[1].toInt(), tokens[2].toInt());
        } else  {
            startListening(tokens[1].toInt());
        }
    } else if (tokens[0] == "listen6") {

        startListening6(tokens[1].toInt());

    } else if (tokens[0] == "connect") {
        addConnection(tokens[1], tokens[2].toInt());
        qDebug() << "Connecting to: " << tokens[1] << ":" << tokens[2];
    } else if (tokens[0] == "send") {
        if (socketList.size() > 0) {
            socketList[0]->write(tokens[1].toLocal8Bit());
            qDebug() << "Wrote: " << tokens[1];
        } else {
            qDebug() << "Not connected";
        }
    } else if (tokens[0] == "stop" && tokens[1] == "listening") {
        server->close();
    } else if (tokens[0] == "list") {
        QNetworkInterface interface;
        qDebug() <<interface.allAddresses();
        qDebug() << interface.allInterfaces();
    } else if (tokens[0] == "ping") {
        protocol->sendPingRequest("192.168.1.1");
    } else if (tokens[0] == "nodeinfo") {
        qDebug() << "Sending node info";
        protocol->sendNodeInfo(QStringList() << "isp1" << "isp2", "12323");
    }

}

void Client::shutDown()
{
    qDebug() << "Shutting down";
}


void Client::error(QAbstractSocket::SocketError error)
{
    qDebug() << "connection problem";
}
