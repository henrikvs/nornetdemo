#include "networklibglobal.h"

#ifndef NODESTATUSMESSAGE_H
#define NODESTATUSMESSAGE_H
#include "abstractmessage.h"

struct NodeStatusStruct {
    int status;
};

class NETWORKLIB_EXPORT NodeStatusMessage : public AbstractMessage
{
public:
    static const int NORMAL = 1;
    static const int UPDATING = 2;
    static const int EXITING = 3;
    static const int RESTARTING = 4;
    NodeStatusMessage();
    NodeStatusMessage(int status);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    NodeStatusStruct data;
};

#endif // NODESTATUSMESSAGE_H
