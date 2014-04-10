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

class DemoCore : public NetworkEntity
{
    Q_OBJECT
public:
    DemoCore();
    void connectToSlivers();
    void installProgram(Sliver *sliver);
    void addSliverConnection(Sliver *sliver);
    void addSliverConnection(QString ip, int port, QString name);
    void start();
    void shutDownNodes();
    void getIpAddress(Sliver *sliver);
    void pingHost(QString sliverName, QString host);
private:
    SliceManager sliceManager;
    QHash<QString, Sliver*> sliverHash;
    QHash<QString, DemoProtocol*> protocolHash;
    Sliver* getSliver(QString name);
    Sliver *getSliver(MyQTcpSocket *socket);
protected:
    virtual AbstractProtocol *createProtocol(int type, MyQTcpSocket *socket);
private slots:
    virtual void connected();
    virtual void connectionError(QAbstractSocket::SocketError error);
    void handleSSHDisconnect();
public slots:
    virtual bool newStdIn(QString input);
    void handleNodeUpdating(MyQTcpSocket *socket);
    void handleNewPingReply(PingReply message, MyQTcpSocket *socket);
    void handleNewNodeInfo(NodeInfoMessage message, MyQTcpSocket *socket);
signals:
    void newStatusMessage(Sliver sliver, NodeInfoMessage message);
    void newPingReply(Sliver sliver, PingReply message);
};

#endif // DEMOCORE_H
