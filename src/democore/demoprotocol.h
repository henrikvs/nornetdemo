#ifndef DEMOPROTOCOL_H
#define DEMOPROTOCOL_H
#include "abstractprotocol.h"
#include "transferstatusmessage.h"
#include "nodeinfomessage.h"
#include "transferstatusmessage.h"
#include "pingreply.h"
class DemoProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit DemoProtocol(QObject *parent = 0);
    void sendDemoStatus();
    void sendExit();
    virtual void start();
    virtual void cleanUp();
    void sendPingRequest(int sessionId, QString remoteIp, QString localIp, int seconds);

    void sendTransferRequest(int id, QString host, QString localIp, int transferType, int seconds);
    void sendStopTask(int sessionId);

    void sendNodeInfoRequest();
protected:
    virtual bool handleMessage(int type);
signals:
    void nodeRestarting(MyQTcpSocket *socket);
    void nodeUpdating(MyQTcpSocket *socket);
    void nodeExiting(MyQTcpSocket *socket);
    void newPingReply(PingReply message, MyQTcpSocket *socket);
    void newNodeInfo(NodeInfoMessage message, MyQTcpSocket *socket);
    void newTransferStatus(TransferStatusMessage message, MyQTcpSocket *socket);
public slots:
    void sendKeepAlive();
};

#endif // DEMOPROTOCOL_H
