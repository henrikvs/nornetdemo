#include <QDebug>
#include "tcpconnection.h"

TcpConnection::TcpConnection(int socketDescriptor, QObject *parent)
    :QObject(parent), socketDescriptor(socketDescriptor)
{
}

TcpConnection::TcpConnection(int socketDescriptor) : socketDescriptor(socketDescriptor)
{

}

void TcpConnection::initiateConnection()
{
    qDebug() << "Running new thread";
    tcpSocket = new QTcpSocket(this);
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
        return;
    }
    qDebug() << "Client connected";
    QByteArray array = "Testing";
    tcpSocket->write(array, array.length());
    tcpSocket->waitForBytesWritten();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotNewMessage()), Qt::QueuedConnection);

}

void TcpConnection::slotNewMessage()
{
    qDebug() << tcpSocket->readAll();

}
