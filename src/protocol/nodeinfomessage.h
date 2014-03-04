#ifndef NODEINFOMESSAGE_H
#define NODEINFOMESSAGE_H
#include "abstractmessage.h"

struct NodeInfoMessageStruct
{
    QStringList ISPConnections;
    QString listeningPort;
};

class NodeInfoMessage : public AbstractMessage
{
public:
    NodeInfoMessage();
    void serialize(QByteArray *block);
    void read(MyQTcpSocket *socket);
    char getType();
    NodeInfoMessageStruct data;
};

#endif // NODEINFOMESSAGE_H
