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
    void installProgram(Sliver sliver);
    void addSliverConnection(Sliver sliver);
    void addSliverConnection(QString ip, int port);
    void start();
    void shutDownNodes();
    void setIpAddress(Sliver sliver);
private:
    SliceManager sliceManager;
    QHash<QString, Sliver> sliverHash;
    QHash<int, DemoProtocol*> protocolHash;
    Sliver getSliver(int socketId);
    void setSliver(Sliver sliver, int id);
protected:
    virtual AbstractProtocol *createProtocol(int type, MyQTcpSocket *socket);
private slots:
    void sliverConnected();
    virtual void connectionError(QAbstractSocket::SocketError error);
public slots:
    virtual bool newStdIn(QString input);
    void pingReplySlot(PingReply message, int id);
    void nodeInfoSlot(NodeInfoMessage message, int id);
    void handleNodeUpdating(int nodeId);
signals:
    void newStatusMessage(Sliver sliver, NodeInfoMessage message);
    void newPingReply(Sliver sliver, PingReply message);
};

#endif // DEMOCORE_H
