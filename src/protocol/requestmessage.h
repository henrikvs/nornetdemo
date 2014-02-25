#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H
#include <QTcpSocket>
#include <QDataStream>
class RequestMessage
{
public:
    RequestMessage();
    void send(QTcpSocket &socket);
    bool receive(QTcpSocket &socket);

    friend QDataStream & operator << ( QDataStream & stream, RequestMessage & msg );
    friend QDataStream & operator >> ( QDataStream & stream, RequestMessage & msg );
private:
    int size;
    QString message;
};

#endif // REQUESTMESSAGE_H
