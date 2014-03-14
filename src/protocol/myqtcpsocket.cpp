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

void MyQTcpSocket::setProtocol(NodeProtocol *protocol)
{
    this->protocol = protocol;
}

NodeProtocol *MyQTcpSocket::getNodeProtocol()
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

ConnectedInfo MyQTcpSocket::getConnectedInfo()
{
    return info;
}
