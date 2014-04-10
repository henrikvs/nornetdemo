#ifndef DEMOPROTOCOL_H
#define DEMOPROTOCOL_H
#include "abstractprotocol.h"
class DemoProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit DemoProtocol(QObject *parent = 0);
    void sendDemoStatus();
    void sendExit();
    virtual void start();
protected:
    virtual bool handleMessage(int type);
signals:
    void nodeRestarting(MyQTcpSocket *socket);
    void nodeUpdating(MyQTcpSocket *socket);
    void nodeExiting(MyQTcpSocket *socket);
    void newPingReply(PingReply message, MyQTcpSocket *socket);
    void newNodeInfo(NodeInfoMessage message, MyQTcpSocket *socket);
public slots:
    void sendKeepAlive();
};

#endif // DEMOPROTOCOL_H
