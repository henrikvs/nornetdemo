#ifndef RELAYPROG_H
#define RELAYPROG_H
#include <QList>
#include <QObject>
#include <QDebug>
#include "networkentity.h"
#include "abstractprotocol.h"
#include "relayprotocol.h"

class RelayProg : public NetworkEntity
{
    Q_OBJECT
public:
    RelayProg();
    virtual void startHandshakeProtocol(int connectionType, QString name, QString remoteName, MyQTcpSocket *socket);
public slots:
    virtual void disconnected(MyQTcpSocket *socket);
    void newKeyboardInput(QString text);
private:
    QHash<QString, RelayProtocol*> pendingNodes;
    QHash<QString, RelayProtocol*> pendingDemos;
    QList<QString> connected;
    virtual int getEntityType();
protected:
    virtual AbstractProtocol *createProtocol(HandshakeMessage message,  MyQTcpSocket *socket);
private slots:
    void exitProgram(int exitValue);
    void handleNewConnection(HandshakeMessage message, RelayProtocol *protocol);
};

#endif // RELAYPROG_H
