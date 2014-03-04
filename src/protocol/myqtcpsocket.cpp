#include "myqtcpsocket.h"

MyQTcpSocket::MyQTcpSocket(QObject *parent) :
    QTcpSocket(parent)
{
    size = -1;
}

void MyQTcpSocket::addSize(int size)
{
    this->size = size;
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
