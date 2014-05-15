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
    iperf4Port = IPERF_IPV4PORT;
    iperf6Port = IPERF_IPV6PORT;
    nextId = 1;
    relayOn = false;
}

NetworkEntity::~NetworkEntity()
{
    qDebug() << "Deconstructing entity";
    /*
    qDebug() << "Destructing";
    foreach (MyQTcpSocket *socket, socketList) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
        socket->close();
        qDebug() << "Disconnected";
    }*/
}

void NetworkEntity::addConnection(QString ip, int port, int type, QString username, QString hostname)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    ConnectionInfo info(ip, port, true,type, hostname, username);
    socket->setConnectionInfo(info);


    setUpSocket(socket);
    if (type == CONNECTION_TYPE_RELAY) {
        socket->isRelay = true;
    }
    socket->connectToHost(ip, port);
   /* MyQTcpSocket *socket = new MyQTcpSocket(this);
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

    socket->connectToHost(ip, port);*/

}


/*
void NetworkEntity::addRelayConnection(QString ip, int port, int type, QString name)
{
    MyQTcpSocket *socket = new MyQTcpSocket(this);
    ConnectionInfo info(ip, port, true,type, name);
    socket->setConnectionInfo(info);
    //AbstractProtocol *protocol = createProtocol(CONNECTION_TYPE_RELAY, socket);
    //socket->setParent(protocol);
    //protocol->setType(type);
    //protocol->setName(getName());
    //protocol->setSocket(socket);
    //protocol->isRelay = true;

    setUpSocket(socket);
    socket->connectToHost(getRelayAddress(), getRelayPort());
}
*/



void NetworkEntity::sendProtocolType(MyQTcpSocket *socket, qint32 type)
{
    type = qToBigEndian<qint32>(type);
    char buffer[sizeof(qint32)];
    memcpy(buffer, &type, sizeof(qint32));
    socket->write(buffer, sizeof(qint32));
}

void NetworkEntity::setUpSocket(MyQTcpSocket *socket)
{
    socket->setId(getNextId());
    socketHash[socket->getId()] = socket;
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
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

void NetworkEntity::startHandshakeProtocol(int connectionType, QString expectedUsername, QString expectedHostname, MyQTcpSocket *socket)
{
    HandshakeProtocol *protocol = new HandshakeProtocol(connectionType, getEntityType(), expectedUsername, expectedHostname, this);
    protocol->setParent(this);
    protocol->setSocket(socket);
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    connect(protocol, SIGNAL(newHandShake(HandshakeMessage,MyQTcpSocket*)), this, SLOT(handleNewHandshake(HandshakeMessage, MyQTcpSocket*)));
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    if (socket->bytesAvailable()) {
        QMetaObject::invokeMethod(protocol, "newData", Qt::QueuedConnection);
    }
    protocol->start();
}



void NetworkEntity::connected()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    ConnectionInfo info = socket->getConnectionInfo();
    startHandshakeProtocol(info.getType(), info.getUsername(), info.getHostName(), socket);
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
    //QThread *thread = new QThread(this);
    socket->setSocketDescriptor(socketDescriptor);

    setUpSocket(socket);
    startHandshakeProtocol(CONNECTION_TYPE_REPLY, "any","any", socket);


    qDebug() << "New connection: " << socket->peerAddress().toString();
}

int NetworkEntity::getNextId() {
    nextId++;
    return nextId;
}

int NetworkEntity::nextRequestId()
{
    static int requestId = 0;
    requestId++;
    return requestId;
}

void NetworkEntity::enableRelay(QString address, int port)
{
    relayOn = true;
    relayAddress = address;
    relayPort = port;
}

void NetworkEntity::setIperf4Port(int port)
{
    qDebug() << "Setting iperf4 port to" << port;
    this->iperf4Port = port;
}

void NetworkEntity::setIperf6Port(int port)
{
    this->iperf6Port = port;
}

int NetworkEntity::getIperf4Port()
{
    return iperf4Port;
}

int NetworkEntity::getIperf6Port()
{
    return iperf6Port;
}

bool NetworkEntity::relayEnabled()
{
    return relayOn;
}

QString NetworkEntity::getRelayAddress()
{
    return relayAddress;
}

int NetworkEntity::getRelayPort()
{
    return relayPort;
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
        //addConnection(tokens[1], tokens[2].toInt());
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
            NetworkInterface interfaceStruct;
            qDebug() << QString("**Interface: %1 \n").arg(interface.humanReadableName());
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach (QNetworkAddressEntry entry, entries) {

                QHostAddress address = entry.ip();
                 qDebug() << (QString("%1: Address: %2 \n").arg(QString::number(i), address.toString()));
                 i++;
            }
        }

    /*} else if (tokens[0] == "ping") {
        int socketId = tokens[1].toInt();

        AbstractProtocol *protocol = socketHash[socketId]->getProtocol();
        protocol->sendPingRequest(tokens[2]);
*/
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

void NetworkEntity::handleNewHandshake(HandshakeMessage message, MyQTcpSocket *socket)
{
    AbstractProtocol *protocol = createProtocol(message, socket);
    socket->setProtocol(protocol);
    connect(socket, SIGNAL(readyRead()), protocol, SLOT(newData()));
    protocol->setSocket(socket);
    protocol->start();
}


void NetworkEntity::connectionError(QAbstractSocket::SocketError error)
{
    qDebug() << "Connection problem";
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
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
    disconnected(socket);
    //socket->waitForBytesWritten(); //TODO avoid waiting
    //socket->close();
    connect(socket, &MyQTcpSocket::destroyed, []() {
       qDebug() << "Socket got destroyed";
    });

    AbstractProtocol *protocol = socket->getProtocol();
    if (protocol != NULL) {
        connect(protocol, &MyQTcpSocket::destroyed, []() {
           qDebug() << "protocol got destroyed";
        });
        protocol->deleteLater();
    }


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
