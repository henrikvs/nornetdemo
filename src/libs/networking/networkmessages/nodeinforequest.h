#include "networklibglobal.h"

#ifndef NODEINFOREQUEST_H
#define NODEINFOREQUEST_H
#include "abstractmessage.h"

class NETWORKLIB_EXPORT NodeInfoRequest : public AbstractMessage
{
public:
    NodeInfoRequest();
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
};

#endif // NODEINFOREQUEST_H
