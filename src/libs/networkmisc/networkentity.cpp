#include "networkentity.h"
#include "myqtcpserver.h"
#include "connectedinfo.h"
#include <QNetworkInterface>
#include <QCoreApplication>
#include <QThread>
#include <QSignalMapper>
#include <QtEndian>
NetworkEntity::NetworkEntity(QObject *parent) :
    QObject(parent)
{
    //connectToFrontEnd("127.0.0.1", 3333);
    //qRegisterMetaType<qintptr>("qintptr");
    nextId = 0;
}

NetworkEntity::~NetworkEntity()
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

void NetworkEntity::addConnection(QString ip, int port, int type, QString name)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    ConnectionInfo info(ip, port, true, name);
    socket->setConnectionInfo(info);
    AbstractProtocol *protocol = createProtocol(type, socket);
    socket->setParent(protocol);
    protocol->setType(type);
    protocol->setSocket(socket);
    socket->setProtocol(protocol);
    socket->setId(getNextId());
    socketHash[socket->getId()] = socket;
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));

    socket->connectToHost(ip, port);
}



void NetworkEntity::connectToNode(QString ip, int port)
{
    addConnection(ip, port);
}

void NetworkEntity::makeConnection(QString ip, int port)
{
    addConnection(ip, port);
}

void NetworkEntity::connectToFrontEnd(QString ip, int port)
{
    addConnection(ip, port);
}

void NetworkEntity::sendProtocolType(MyQTcpSocket *socket, qint32 type)
{
    type = qToBigEndian<qint32>(type);
    char buffer[sizeof(qint32)];
    memcpy(buffer, &type, sizeof(qint32));
    socket->write(buffer, sizeof(qint32));
}

void NetworkEntity::startListening(int port)
{
    //QThread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    MyQTcpServer *server = new MyQTcpServer(this);
    serverList << server;
    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(QHostAddress::AnyIPv4, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() << "id: " << serverList.length()-1 ;
    //thread->start();
    //server->listen(QHostAddress::Any, port);
}

void NetworkEntity::startListening6(int port)
{
    //QThread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    MyQTcpServer *server = new MyQTcpServer(this);
    serverList << server;

    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(QHostAddress::AnyIPv6, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() << "id: " << serverList.length()-1 ;
    //thread->start();
    //server->listen(QHostAddress::Any, port);
}

void NetworkEntity::startListening(int port, int interface)
{
    QNetworkInterface inter;
    //Thread *thread = new QThread(this);
    //server->moveToThread(thread);
    //server->listConnections();
    //connect(thread, SIGNAL(started()), server, SLOT(listen()));
    //MyQTcpServer *server = new MyQTcpServer(this);
    MyQTcpServer *server = new MyQTcpServer(this);
    serverList << server;
    connect(server, SIGNAL(newConnection(qintptr)), this, SLOT(newConnection(qintptr)));
    //connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    server->listen(inter.allAddresses()[interface], port);
    //thread->start();
    //server->listen(QHostAddress::Any, port);
    qDebug() << "Listening on port: " << port << " interface: " << server->serverAddress().toString() << "id: " << serverList.length()-1 ;
}

void NetworkEntity::stopListening()
{
    qDebug() << "Stopping server";
    //server->stopListening();
}

MyQTcpSocket *NetworkEntity::getSocket(int id)
{
    return socketHash[id];
}



void NetworkEntity::connected()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    AbstractProtocol *protocol = socket->getProtocol();
    qDebug() << "connected: " << socket->getId();
    qDebug() << "Sending protocol type: " << protocol->getType();
    sendProtocolType(socket, protocol->getType()); //TEMP
    protocol->start();
}

void NetworkEntity::disconnected()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    qDebug() << "disconnected: " << socket->getId();
    removeSocket(socket->getId());
}

void NetworkEntity::newConnection(qintptr socketDescriptor)
{
    MyQTcpSocket *socket = new MyQTcpSocket;
    socket->setId(getNextId());
    //QThread *thread = new QThread(this);
    socket->setSocketDescriptor(socketDescriptor);
    socketHash[socket->getId()] = socket;
    //socket->moveToThread(thread);
    //protocol->moveToThread(thread);
    //thread->start();
    //connect(thread, SIGNAL(started()), protocol, SLOT());
    connect(socket, SIGNAL(readyRead()), this, SLOT(newData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));

    qDebug() << "New connection: " << socket->peerAddress().toString();
}

int NetworkEntity::getNextId() {
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
void NetworkEntity::newData()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    qint32 type;
    char buffer[sizeof(qint32)];
    if (socket->bytesAvailable() >= sizeof(qint32)) {
        socket->read(buffer, sizeof(qint32));
        memcpy(&type, buffer, sizeof(qint32));
        type = qFromBigEndian<qint32>(type);
    } else {
        return;
    }

    AbstractProtocol *protocol = createProtocol(type, socket);
    socket->setParent(protocol);
    socket->setProtocol(protocol);
    protocol->setSocket(socket);

    //if (type == PROTOCOL_TYPE_TEST) {
        qDebug() << "New protocol!" << "Socket index: " << socket->getId();
    //}
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(newData()));
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData())); //now connect the stream to the protocol
    protocol->start();
    if (socket->bytesAvailable() > 0) {
        QMetaObject::invokeMethod(protocol, "newData", Qt::QueuedConnection);//in case there's more to read
    }
}

bool NetworkEntity::newStdIn(QString input)
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
        qDebug() << "stopping listening";
        serverList[tokens[2].toInt()]->close();
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

        AbstractProtocol *protocol = socketHash[socketId]->getProtocol();
        protocol->sendPingRequest(tokens[2]);

    } else if (tokens[0] == "getinfo") {
        qDebug() << "Getting node info";
        int socketId = tokens[1].toInt();
        AbstractProtocol *protocol = socketHash[socketId]->getProtocol();
        protocol->sendNodeInfoRequest();
    } else {
        return false;
    }
    return true;
}

void NetworkEntity::shutDown(int exitValue)
{
    qDebug() << "Shutting down";
    foreach (MyQTcpSocket *socket, socketHash) {
        disconnectSocket(socket->getId());
    }
    foreach (MyQTcpServer *server, serverList) {
        if (server != NULL) {
            server->close();
        }
        //delete(server);
    }

    emit shutDownComplete(exitValue);
}

void NetworkEntity::connectionError(QAbstractSocket::SocketError error)
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    AbstractProtocol *protocol = socket->getProtocol();
    qDebug() << "Connection error:" << socket->getConnectionInfo().getAddress();
    removeSocket(socket->getId());
}

void NetworkEntity::removeSocket(int socketId)
{
    qDebug() << "removeSocket(int socketId)";
    if (!socketHash.contains(socketId)) {
        return;
    }
    qDebug() << "Removing socket: " << socketId;
    MyQTcpSocket *socket = socketHash[socketId];
    //socket->waitForBytesWritten(); //TODO avoid waiting
    //socket->close();
    connect(socket, &MyQTcpSocket::destroyed, []() {
       qDebug() << "Socket got destroyed";
    });

    AbstractProtocol *protocol = socket->getProtocol();
    connect(protocol, &MyQTcpSocket::destroyed, []() {
       qDebug() << "protocol got destroyed";
    });
    protocol->deleteLater();


    socketHash.remove(socketId);
}

void NetworkEntity::disconnectSocket(int socketId)
{
    MyQTcpSocket *socket = socketHash[socketId];
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected()) {
        removeSocket(socketId);
    }
}
