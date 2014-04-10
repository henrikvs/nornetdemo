#ifndef REQUESTMESSAGE_H
#define REQUESTMESSAGE_H
#include <QTcpSocket>
#include <QDataStream>
class RequestMessage
{
public:
    static const int SIZE_MSGSIZE = (int)sizeof(quint16);
    static const int SIZE_MSGTYPE = (int)sizeof(quint16);
    enum { MSGTYPE_PING, MSGTYPE_COMMANDREQUEST};
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
