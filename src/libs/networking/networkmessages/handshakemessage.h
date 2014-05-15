#include "networklibglobal.h"

#ifndef HANDSHAKEMESSAGE_H
#define HANDSHAKEMESSAGE_H
#include <QByteArray>
#include "abstractmessage.h"

struct HandshakeStruct {
    QString username;
    QString hostname;
    QString expectedUsername;
    QString expectedHostname;
    quint16 connectionType;
    quint16 entityType;
};

class NETWORKLIB_EXPORT HandshakeMessage : public AbstractMessage
{
public:
    HandshakeMessage();
    HandshakeMessage(QString username, QString hostname, QString expectedUsername, QString expectedHostname, int connectionType, int entityType);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    HandshakeStruct data;
};

#endif // HANDSHAKEMESSAGE_H
