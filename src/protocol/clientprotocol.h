#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H
#include <QObject>
#include <QTcpSocket>

class ClientProtocol : public QObject
{
public:
    ClientProtocol();
    void start(QTcpSocket &socket);
signals:
    //void signalNewRequest(RequestMessage &request);

};

#endif // CLIENTPROTOCOL_H
