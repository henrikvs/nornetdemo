#include "myqtcpsocket.h"

MyQTcpSocket::MyQTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    size = -1;
    protocol = NULL;
}

void MyQTcpSocket::addSize(int size)
{
    this->size = size;
}

void MyQTcpSocket::setId(int id)
{
    this->id = id;
}

void MyQTcpSocket::setProtocol(AbstractProtocol *protocol)
{
    this->protocol = protocol;
}

void MyQTcpSocket::setConnectionInfo(ConnectionInfo info)
{
    this->info = info;
}

ConnectionInfo MyQTcpSocket::getConnectionInfo()
{
    return info;
}

AbstractProtocol *MyQTcpSocket::getProtocol()
{
    return protocol;
}



int MyQTcpSocket::getId()
{
    return id;
}

bool MyQTcpSocket::sizeRead()
{
    if (size>-1) return true;
    return false;
}

void MyQTcpSocket::resetSize()
{
    size=-1;
}

