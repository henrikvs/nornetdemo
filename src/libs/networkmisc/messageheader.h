#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
#include "abstractmessage.h"
#include <QDataStream>

struct MessageHeaderStruct
{
    quint16 size;
    quint8 type;
    quint32 seqNum;
};

class MessageHeader : public AbstractMessage
{
public:
    static const int SIZE = sizeof(quint16) + sizeof(quint8) + sizeof(quint32); //Remember to update this
    MessageHeader();
    MessageHeader(int size, int type, int seqNum);
    void serialize(QByteArray *block) const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    MessageHeaderStruct data;
};

#endif // MESSAGEHEADER_H
