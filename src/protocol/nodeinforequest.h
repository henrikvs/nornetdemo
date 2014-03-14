#ifndef NODEINFOREQUEST_H
#define NODEINFOREQUEST_H
#include "abstractmessage.h"

class NodeInfoRequest : public AbstractMessage
{
public:
    NodeInfoRequest();
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // NODEINFOREQUEST_H
