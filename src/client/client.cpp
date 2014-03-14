#include "client.h"
#include "myqtcpserver.h"
#include <QNetworkInterface>
#include <QCoreApplication>
#include <QThread>
#include <QSignalMapper>
Client::Client(QObject *parent) :
    QObject(parent)
{
    //connectToFrontEnd("127.0.0.1", 3333);
    //qRegisterMetaType<qintptr>("qintptr");
    server = new MyQTcpServer(this);
    nextId = 0;
}

Client::~Client()
{
    /*
    qDebug() << "Destructing";
    foreach (MyQTcpSocket *socket, socketList) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
        socket->close();
        qDebug() << "Disconnected";
    }*/
}

void Client::addConnection(QString ip, int port)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    NodeProtocol *protocol = new NodeProtocol(socket, getNextId(), this);
    protocol->setHostName(ip);
    protocol->setPort(port);
    socket->setProtocol(protocol);
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
    NodeProtocol *protocol = socket->getNodeProtocol();
    socketHash[protocol->getId()] = socket;


    qDebug() << "connected: " << protocol->getId();
}

void Client::newConnection(qintptr socketDescriptor)
{
    MyQTcpSocket *socket = new MyQTcpSocket;
    int id = getNextId();
    NodeProtocol *protocol = new NodeProtocol(socket, id);
    //QThread *thread = new QThread(this);
    socket->setSocketDescriptor(socketDescriptor);
    //socket->moveToThread(thread);
    //protocol->moveToThread(thread);
    //thread->start();
    //connect(thread, SIGNAL(started()), protocol, SLOT());
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    socket->setProtocol(protocol);
    socketHash[id] = socket;
    qDebug() << "New connection: " << socket->peerAddress().toString() << ": " << protocol->getId();
}

int Client::getNextId() {
    nextId++;
    return nextId;
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
    } else if (tokens[0] == "stop" && tokens[1] == "listening") {
        server->close();
    } else if (tokens[0] == "list") {
        QNetworkInterface interface;
        qDebug() <<interface.allAddresses();
        qDebug() << interface.allInterfaces();
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        int i = 0;
        foreach (QNetworkInterface interface, interfaces) {
            Interface interfaceStruct;
            qDebug() << QString("**Interface: %1 \n").arg(interface.humanReadableName());
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach (QNetworkAddressEntry entry, entries) {

                QHostAddress address = entry.ip();
                 qDebug() << (QString("%1: Address: %2 \n").arg(QString::number(i), address.toString()));
                 i++;
            }
        }

    } else if (tokens[0] == "ping") {
        int socketId = tokens[1].toInt();
        NodeProtocol *protocol = socketHash[socketId]->getNodeProtocol();
        protocol->sendPingRequest(tokens[2]);

    } else if (tokens[0] == "getinfo") {
        qDebug() << "Getting node info";
        int socketId = tokens[1].toInt();
        NodeProtocol *protocol = socketHash[socketId]->getNodeProtocol();
        protocol->sendNodeInfoRequest();
    }

}

void Client::shutDown()
{
    qDebug() << "Shutting down";
}

void Client::newNodeInfo_slot(NodeInfoMessage message, int id)
{
    emit newNodeInfo_signal(message, id);
}

void Client::newPingReply_slot(PingReply message, int id)
{
    emit newPingReply_signal(message, id);
}


void Client::error(QAbstractSocket::SocketError error)
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    NodeProtocol *protocol = socket->getNodeProtocol();
    emit connectionError(protocol->getHostName(), protocol->getPort());
}
