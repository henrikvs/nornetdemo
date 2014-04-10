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
protected:
    virtual AbstractProtocol *createProtocol(int type, MyQTcpSocket *socket);
private slots:
    void failedToConnect(QString host, int port);
    void connected(QString host, int port);
    void exitProgram(int exitValue);
};

#endif // RELAYPROG_H
