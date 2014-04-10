#include "relayprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
#include <QTimer>
RelayProtocol::RelayProtocol(QObject *parent) : AbstractProtocol(parent)
{

}


void RelayProtocol::start()
{
    qDebug() << "Starting protocol";
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    timer->start(10000);*/
}



bool RelayProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;
    } else if (type == AbstractProtocol::MSGTYPE_NODESTATUSMESSAGE) {
	}
    return true;
}
