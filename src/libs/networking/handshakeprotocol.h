#include "networklibglobal.h"

#ifndef HANDSHAKEPROTOCOL_H
#define HANDSHAKEPROTOCOL_H
#include "abstractprotocol.h"
#include "handshakemessage.h"
class NETWORKLIB_EXPORT HandshakeProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit HandshakeProtocol(int connectionType, int entityType,  QString expectedUsername, QString expectedHostname, QObject *parent = 0);
    virtual void start();
    virtual void cleanUp();
protected:
    virtual bool handleMessage(int type);
private:
    int connectionType;
    QString expectedUsername;
    QString expectedHostname;
    bool active;
    int entityType;
signals:
    void newHandShake(HandshakeMessage message, MyQTcpSocket *socket);

public slots:
};

#endif // HANDSHAKEPROTOCOL_H
