#ifndef DEMOCORE_H
#define DEMOCORE_H
#include <QList>
#include <QObject>
#include <QDebug>
#include <QHash>
#include "slicemanager.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "demoprotocol.h"
#include "sliver.h"
#include "transferstatusmessage.h"

class DemoCore : public NetworkEntity
{
    Q_OBJECT
public:
    DemoCore();
    void connectToSlivers(QList<Sliver*> slivers);
    void installProgram(Sliver *sliver);
    void addSliverConnection(Sliver *sliver);
    void addSliverConnection(QString ip, int port, QString sliceName, QString hostName);
    void start();
    void shutDownNodes();
    void getIpAddress(Sliver *sliver);
    void pingHost(QString sliverName, QString host, QString localIp);
    int transferRequest(QString sliverName, QString host, QString localIp, int transferType, int seconds);
    void enableGatekeeper(QString username, QString hostname);
    void disableGatekeeper();
    virtual void disconnected(MyQTcpSocket *socket);

    QStringList getIpv4List(QString name);
    QStringList getIpv6List(QString name);
private:
    QString gatekeeperUsername;
    QString gatekeeperHostname;
    bool gateKeeperEnabled;
    SliceManager sliceManager;
    QHash<QString, Sliver*> sliverHash;
    QHash<QString, DemoProtocol*> protocolHash;
    Sliver* getSliver(QString name);
    Sliver *getSliver(MyQTcpSocket *socket);
    virtual int getEntityType();
protected:
    virtual AbstractProtocol *createProtocol(HandshakeMessage message, MyQTcpSocket *socket);
private slots:
    virtual void connected();
    void handleSSHDisconnect();
public slots:
    virtual bool newStdIn(QString input);
    void handleNodeUpdating(MyQTcpSocket *socket);
    void handleNewPingReply(PingReply message, MyQTcpSocket *socket);
    void handleNewNodeInfo(NodeInfoMessage message, MyQTcpSocket *socket);
    void handleNewTransferStatus(TransferStatusMessage message, MyQTcpSocket *socket);
signals:
    void newStatusMessage(Sliver sliver, NodeInfoMessage message);
    void newPingReply(Sliver sliver, PingReply message);
    void newTransferStatus(Sliver sliver, TransferStatusMessage message);
};

#endif // DEMOCORE_H
