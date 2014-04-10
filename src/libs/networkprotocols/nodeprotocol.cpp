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





NodeProtocol::NodeProtocol(QObject *parent) : AbstractProtocol(parent)
{

}

bool NodeProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;

    } else if (type == MSGTYPE_PINGREQUEST) {
            PingRequest message;
            message.read(socket);
            qDebug() << "Remote ip: " << message.data.remoteIp;


            PingTask *task = new PingTask(message.data.remoteIp, this);
            connect(task, &PingTask::newPing, this, &AbstractProtocol::sendMessage);
            connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
            task->start();
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
    } else if (type == MSGTYPE_NODEINFOREQUEST) {
        PingRequest message;
        message.read(socket);
        InfoTask *task = new InfoTask(this);
        connect(task, &InfoTask::newInfoMessage, this, &AbstractProtocol::sendMessage);
        connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
        task->start();
    } else {
        return false;
    }
    return true;
}




