#ifndef NODEPROTOCOL_H
#define NODEPROTOCOL_H
#include <QObject>
#include <QTcpSocket>
#include "myqtcpsocket.h"
#include "abstractmessage.h"
#include "abstractprotocol.h"
#include "messageheader.h"
#include "pingreply.h"
#include "nodeinfomessage.h"
#include <QtEndian>

class AbstractTask;

class NodeProtocol : public AbstractProtocol
{
    Q_OBJECT
public:
    void sendNodeInfo();
    void sendUpdatingStatus();
    void sendNodeInfo(QStringList isps, QString port);
    void sendNormalStatus();
    virtual void start();
    virtual void cleanUp();


    explicit NodeProtocol(int iperfIpv4Port, int iperfIpv6Port, QObject *parent = 0);
protected:
    virtual bool handleMessage(int type);
signals:
    void exitProgram(int id);
private:
    int iperfIpv4Port;
    int iperfIpv6Port;
    QHash<int, AbstractTask*> tasks;

};

#endif // CLIENTPROTOCOL_H
