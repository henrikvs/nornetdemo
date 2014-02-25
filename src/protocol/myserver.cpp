#include "server.h"
#include <QDebug>
Server::Server(QObject *parent) : QTcpServer(parent)
{

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "New connection";
    QThread *thread = new QThread(this);
    TcpConnection *connection = new TcpConnection(socketDescriptor);
    //connection->moveToThread(thread);
    //connect(thread, SIGNAL(started()), connection, SLOT(initiateConnection()));
    //connect(connection, SIGNAL(finished()), thread, SLOT(quit()));
    //thread->start();
    connection->initiateConnection();
}
