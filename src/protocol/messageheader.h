#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
#include "abstractmessage.h"
#include <QDataStream>

struct MessageHeaderStruct
{
    quint16 size;
    quint8 type;
};

class MessageHeader : public AbstractMessage
{
public:
    static const int SIZE = sizeof(quint16) + sizeof(quint8); //Remember to update this
    MessageHeader();
    MessageHeader(int size, int type);
    void serialize(QByteArray *block);
    void read(MyQTcpSocket *socket);
    char getType();
    MessageHeaderStruct data;
};

#endif // MESSAGEHEADER_H
