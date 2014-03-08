#ifndef ABSTRACTMESSAGE_H
#define ABSTRACTMESSAGE_H

#include <QByteArray>
#include "myqtcpsocket.h"
class AbstractMessage : public QObject
{
    Q_OBJECT
public:
    AbstractMessage(QObject *parent = 0);
    virtual void serialize(QByteArray *block) = 0;
    virtual void read(MyQTcpSocket *socket) = 0;
    virtual char getType() = 0;
};

#endif // ABSTRACTMESSAGE_H
