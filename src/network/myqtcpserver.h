#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class MyQTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyQTcpServer(QObject *parent = 0);

signals:
    void newConnection(qintptr socketDescriptor);

public slots:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // SERVER_H
