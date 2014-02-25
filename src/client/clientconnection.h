#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

#include <QDebug>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    ClientConnection(QObject *parent);
    ClientConnection();
    void setHost(QString host, int port);
    void initiateConnection();
private:
    QNetworkSession *networkSession;
    QTcpSocket *tcpSocket;
    QString host;
    int port;

private slots:
    void sessionOpen();
    void error(QAbstractSocket::SocketError error);
    void connected();
    void readData();
    void writeData(QByteArray array);
signals:
    //void connectionOpen(QTcpSocket);
};

#endif // CLIENT_H
