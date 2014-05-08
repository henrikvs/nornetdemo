#include "stringmessage.h"
StringMessage::StringMessage()
{
}

StringMessage::StringMessage(QString text)
{
    this->data.text = text;
}

void StringMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.text;
}

void StringMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.text;
}

char StringMessage::getType() const
{
    return MSGTYPE_STRINGMESSAGE;
}
