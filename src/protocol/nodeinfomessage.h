#ifndef NODEINFOMESSAGE_H
#define NODEINFOMESSAGE_H
#include "abstractmessage.h"

struct Interface
{
    QString interfaceName;
    QStringList addresses;
};

struct NodeInfoMessageStruct
{
    QList<Interface> interfaces;
    QString listeningPort;
};


class NodeInfoMessage : public AbstractMessage
{
    Q_OBJECT
public:
    NodeInfoMessage(QObject *parent = 0);
    void serialize(QByteArray *block);
    void read(MyQTcpSocket *socket);
    char getType();
    NodeInfoMessageStruct data;
};

#endif // NODEINFOMESSAGE_H
