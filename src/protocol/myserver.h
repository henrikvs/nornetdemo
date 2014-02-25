#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include "tcpconnection.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);

signals:

public slots:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // SERVER_H
