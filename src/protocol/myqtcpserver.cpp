#include "myqtcpserver.h"
#include <QDebug>
MyQTcpServer::MyQTcpServer(QObject *parent) : QTcpServer(parent)
{

}

void MyQTcpServer::incomingConnection(qintptr socketDescriptor)
{
   // qDebug() << "New connection";
    //QThread *thread = new QThread(this);
    //TcpConnection *connection = new TcpConnection(socketDescriptor);
    //connection->moveToThread(thread);
    //connect(thread, SIGNAL(started()), connection, SLOT(initiateConnection()));
    //connect(connection, SIGNAL(finished()), thread, SLOT(quit()));
    //thread->start();
    //connection->initiateConnection();
    emit newConnection(socketDescriptor);
}
