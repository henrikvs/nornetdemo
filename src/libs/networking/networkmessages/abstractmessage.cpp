#include "abstractmessage.h"

AbstractMessage::AbstractMessage()
{
}

void AbstractMessage::serialize(QByteArray *) const
{

}

void AbstractMessage::read(MyQTcpSocket *)
{

}

char AbstractMessage::getType() const
{
    return 0;
}
