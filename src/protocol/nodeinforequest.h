#ifndef NODEINFOREQUEST_H
#define NODEINFOREQUEST_H
#include "abstractmessage.h"

class NodeInfoRequest: public AbstractMessage
{
    Q_OBJECT
public:
    NodeInfoRequest(QObject *parent = 0);
    void serialize(QByteArray *block);
    void read(MyQTcpSocket *socket);
    char getType();
};

#endif // NODEINFOREQUEST_H
