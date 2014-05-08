#include "networklibglobal.h"

#ifndef STRINGMESSAGE_H
#define STRINGMESSAGE_H
#include <QString>
#include "myqtcpsocket.h"
#include "abstractmessage.h"
struct StringMessageStruct {
    QString text;
};

class NETWORKLIB_EXPORT StringMessage : public AbstractMessage
{
public:
    StringMessage();
    StringMessage(QString text);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    StringMessageStruct data;
};

#endif // STRINGMESSAGE_H
