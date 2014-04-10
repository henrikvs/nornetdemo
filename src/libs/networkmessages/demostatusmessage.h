#ifndef DEMOSTATUSMESSAGE_H
#define DEMOSTATUSMESSAGE_H
#include <QByteArray>
#include "abstractprotocol.h"

struct DemoStatusStruct {
    int version;
};

class DemoStatusMessage : public AbstractMessage
{
public:
    DemoStatusMessage();
    DemoStatusMessage(int version);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    DemoStatusStruct data;
};

#endif // DEMOSTATUSMESSAGE_H
