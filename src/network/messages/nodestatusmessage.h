#ifndef NODESTATUSMESSAGE_H
#define NODESTATUSMESSAGE_H
#include "abstractmessage.h"

struct NodeStatusStruct {
    int status;
};

class NodeStatusMessage : public AbstractMessage
{
public:
    static const int NORMAL = 1, UPDATING = 2, EXITING = 3, RESTARTING = 4;
    NodeStatusMessage();
    NodeStatusMessage(int status);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    NodeStatusStruct data;
};

#endif // NODESTATUSMESSAGE_H
