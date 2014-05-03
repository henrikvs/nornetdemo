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

class NodeProtocol : public AbstractProtocol
{
    Q_OBJECT
public:
    void sendNodeInfo();
    void sendUpdatingStatus();
    void sendNormalStatus();
    virtual void start();
    virtual void cleanUp();


    explicit NodeProtocol(QObject *parent = 0);
protected:
    virtual bool handleMessage(int type);
signals:
    void exitProgram(int id);


};

#endif // CLIENTPROTOCOL_H
