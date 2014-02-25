#include "clientconnection.h"

ClientConnection::ClientConnection(QObject *parent) : QObject(parent)
{
    //QNetworkConfigurationManager manager;

    //QNetworkConfiguration config;
    //config = manager.defaultConfiguration();
    //networkSession = new QNetworkSession(config, this);
    tcpSocket = new QTcpSocket(this);
    //QObject::connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpen()));
    QObject::connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

ClientConnection::ClientConnection()
{

}

void ClientConnection::setHost(QString host, int port)
{
    this->host = host;
    this->port = port;
}

void ClientConnection::sessionOpen()
{
    qDebug()<< "Session is open";
    tcpSocket->connectToHost(host, port);
}

void ClientConnection::error(QAbstractSocket::SocketError error)
{
    qDebug()<< "Error connecting";
}

void ClientConnection::connected()
{
    qDebug() << "Connected";

}

void ClientConnection::readData()
{
    QByteArray array = tcpSocket->readAll();
    qDebug() << array;
    tcpSocket->write("Replying");
}

void ClientConnection::writeData(QByteArray array)
{
    tcpSocket->write(array);
}


void ClientConnection::initiateConnection()
{
    networkSession->open();
}
