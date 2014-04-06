#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include <QByteArray>
#include "myqtcpsocket.h"
class AbstractMessage
{
public:
    AbstractMessage();
    virtual void serialize(QByteArray *block) const;
    virtual void read(MyQTcpSocket *socket) ;
    virtual char getType() const;
};

#endif // ABSTRACTMESSAGE_H