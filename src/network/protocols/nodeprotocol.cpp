#include "nodeprotocol.h"
#include "pingreply.h"
#include "pingrequest.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "nodeinfomessage.h"
#include "nodeinforequest.h"
#include "pingtask.h"
#include "infotask.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "nodeinfomessage.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"

void NodeProtocol::sendNodeInfo()
{
    InfoTask *task = new InfoTask(this);
    connect(task, &InfoTask::newInfoMessage, this, &AbstractProtocol::sendMessage);
    connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
    task->start();
}

void NodeProtocol::sendUpdatingStatus()
{
    NodeStatusMessage message(NodeStatusMessage::UPDATING);
    sendMessage(message);
}

void NodeProtocol::sendNormalStatus()
{
    NodeStatusMessage message(NodeStatusMessage::NORMAL);
    sendMessage(message);
}

void NodeProtocol::start()
{

}





NodeProtocol::NodeProtocol(int id, QObject *parent) : AbstractProtocol(id, parent)
{

}

bool NodeProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;
    } else if (type == MSGTYPE_DEMOSTATUS) {
        qDebug() << "Received demostatus";
        DemoStatusMessage message;
        message.read(socket);
        if (message.data.version != NetworkEntity::VERSION) {
            qDebug() << "Wrong version, updating";
            sendUpdatingStatus();
            emit exitProgram(NetworkEntity::EXIT_TYPE_UPDATE);
        } else {
            qDebug() << "Send normal status";
            sendNormalStatus();
        }
    } else if (type == MSGTYPE_STRINGMESSAGE) {
        StringMessage message;
        message.read(socket);
        QString command = message.data.text;
        if (command == "exit") {
            emit exitProgram(NetworkEntity::EXIT_TYPE_NORMAL);
        }
    } else {
        return false;
    }
    return true;
}




