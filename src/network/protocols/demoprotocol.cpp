#include "demoprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
DemoProtocol::DemoProtocol(int id, QObject *parent) : AbstractProtocol(id, parent)
{

}

void DemoProtocol::sendDemoStatus()
{
    qDebug() << "Sending demo status";
    DemoStatusMessage message(NetworkEntity::VERSION);
    sendMessage(message);
}

void DemoProtocol::sendExit()
{
    StringMessage message("exit");
    sendMessage(message);

}

void DemoProtocol::start()
{
    qDebug() << "Starting protocol";
    sendDemoStatus();
}



bool DemoProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;
    } else if (type == AbstractProtocol::MSGTYPE_NODESTATUSMESSAGE) {
        NodeStatusMessage message;
        message.read(socket);
        if (message.data.status == NodeStatusMessage::NORMAL) {
            sendNodeInfoRequest();
            qDebug() << "Received OK message from node";
        } else if (message.data.status == NodeStatusMessage::UPDATING) {
            qDebug() << "Received updating message from node";
            emit nodeUpdating(getId());
        } else if (message.data.status == NodeStatusMessage::EXITING) {
            emit nodeExiting(getId());
        } else if (message.data.status == NodeStatusMessage::RESTARTING) {
            emit nodeRestarting(getId());
        }
    } else {
        return false;
    }
    return true;
}
