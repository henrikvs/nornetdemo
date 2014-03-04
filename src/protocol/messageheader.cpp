#include "messageheader.h"

MessageHeader::MessageHeader()
{
}

MessageHeader::MessageHeader(int size, int type)
{
    data.size = size;
    data.type = type;
}

void MessageHeader::serialize(QByteArray *block)
{
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.size << data.type;

}

void MessageHeader::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.size >> data.type;
}

char MessageHeader::getType()
{
    return 0;
}
