#ifndef NODEINFOMESSAGE_H
#define NODEINFOMESSAGE_H
#include "abstractmessage.h"

struct NetworkInterface
{
    QString interfaceName;
    QStringList addresses;
};

struct NodeInfoMessageStruct
{
    QList<NetworkInterface> interfaces;
    QString listeningPort;
    QString lat;
    QString lng;

};


class NodeInfoMessage : public AbstractMessage
{
public:

    NodeInfoMessage();
    void serialize(QByteArray *block) const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    NodeInfoMessageStruct data;
};

#endif // NODEINFOMESSAGE_H
