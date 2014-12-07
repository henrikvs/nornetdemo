#ifndef RELAYPROTOCOL_H
#define RELAYPROTOCOL_H
#include "relayprotocol.h"
#include "abstractprotocol.h"
#include "handshakemessage.h"
class RelayProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit RelayProtocol(QObject *parent = 0);
    virtual void start();
    void setRemoteProtocol(RelayProtocol *protocol);
    void remoteDisconnected();
    virtual void cleanUp();
    void sendData(QByteArray &buffer, MyQTcpSocket *sock);
    HandshakeMessage getHandshakeMessage();
protected:
    virtual bool handleMessage(int type);
private:
    RelayProtocol *remoteProtocol;
    QByteArray buffer;
    HandshakeMessage handshakeMessage;
    void setHandshakeMessage(HandshakeMessage message);
signals:
    void newConnection(HandshakeMessage message, RelayProtocol *protocol);
public slots:
};

#endif // RELAYPROTOCOL_H
