#ifndef NODEPROG_H
#define NODEPROG_H
#include <QList>
#include <QObject>
#include <QDebug>
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodeprotocol.h"

class QProcess;

class NodeProg : public NetworkEntity
{
    Q_OBJECT
public:
    NodeProg();
    void start(int port = 33555);
    virtual void disconnected(MyQTcpSocket *socket);
protected:
    virtual AbstractProtocol *createProtocol(HandshakeMessage message, MyQTcpSocket *socket);
public slots:
private slots:
    void exitProgram(int exitValue);
private:
    virtual int getEntityType();
    void startIperf(int port);
    void startIperfIpv6(int port);
    QProcess *iperf4Process = NULL;
    QProcess *iperf6Process = NULL;
};

#endif // NODEPROG_H
