#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include <QThread>
#include <QtNetwork/QTcpSocket>

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    TcpConnection(int socketDescriptor, QObject *parent);
    TcpConnection(int socketDescriptor);
signals:
    void error(QTcpSocket::SocketError socketError);
    void finished();

private:
    int socketDescriptor;
    QTcpSocket *tcpSocket;
public slots:
    void slotNewMessage();
    void initiateConnection();
};

#endif // SERVERTHREAD_H
