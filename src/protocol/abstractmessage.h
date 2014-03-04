#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include <QByteArray>
#include "myqtcpsocket.h"
class AbstractMessage
{
public:
    AbstractMessage();
    virtual void serialize(QByteArray *block) = 0;
    virtual void read(MyQTcpSocket *socket) = 0;
    virtual char getType() = 0;
    QDataStream out;
    QDataStream in;
};

#endif // ABSTRACTMESSAGE_H
