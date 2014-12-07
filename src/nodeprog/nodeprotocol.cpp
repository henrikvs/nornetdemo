#include <QTimer>
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
#include "transferrequestmessage.h"
#include "transfertask.h"
#include "taskcontrollermessage.h"

void NodeProtocol::sendNodeInfo()
{
    InfoTask *task = new InfoTask(this);
    connect(task, &InfoTask::newInfoMessage, this, &AbstractProtocol::sendMessage);
    connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
    task->start();
}

void NodeProtocol::sendNodeInfo(QStringList isps, QString port) {
    NodeInfoMessage message;
    //message.data.ISPConnections = isps;
    message.data.listeningPort = port;
    sendMessage(message);
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
    AbstractProtocol::start();
    this->iperfIpv4Port = iperfIpv4Port;
    this->iperfIpv6Port = iperfIpv6Port;
}

void NodeProtocol::cleanUp()
{

}





NodeProtocol::NodeProtocol(int iperfIpv4Port, int iperfIpv6Port, QObject *parent) : AbstractProtocol(parent)
{
    this->iperfIpv4Port = iperfIpv4Port;
    this->iperfIpv6Port = iperfIpv6Port;
}

bool NodeProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;

    } else if (type == AbstractMessage::MSGTYPE_PINGREQUEST) {
        PingRequest message;
        message.read(socket);
        qDebug() << "Recevied ping: " << message.data.remoteIp <<message.data.seconds;

        PingTask *task = new PingTask(message.data.sessionId, message.data.remoteIp, message.data.localIp, message.data.seconds, this);
        connect(task, &PingTask::newPing, this, &AbstractProtocol::sendMessage);
        connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
        tasks[message.data.sessionId] = task;
        task->start();
    } else if (type == AbstractMessage::MSGTYPE_DEMOSTATUS) {
        qDebug() << "Received demostatus";
        DemoStatusMessage message;
        message.read(socket);
        if (message.data.version > NetworkEntity::VERSION) {
            qDebug() << "Wrong version, updating";
            sendUpdatingStatus();
            emit exitProgram(NetworkEntity::EXIT_TYPE_UPDATE);
        } else {
            qDebug() << "Send normal status";
            sendNormalStatus();
        }
    } else if (type == AbstractMessage::MSGTYPE_STRINGMESSAGE) {
        StringMessage message;
        message.read(socket);
        QString command = message.data.text;
        if (command == "exit") {
            qDebug() << "Received exit message";
            emit exitProgram(NetworkEntity::EXIT_TYPE_NORMAL);
        }
    } else if (type == AbstractMessage::MSGTYPE_NODEINFOREQUEST) {
        PingRequest message;
        message.read(socket);
        InfoTask *task = new InfoTask(this);
        connect(task, &InfoTask::newInfoMessage, this, &AbstractProtocol::sendMessage);
        connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
        task->start();
    } else if (type == AbstractMessage::MSGTYPE_TRANSFERREQUEST) {
        TransferRequestMessage message;
        message.read(socket);
        int port = 0;
        if (message.data.localIp.contains(".")) { //if the address is an ipv4 address
            port = iperfIpv4Port;
        } else {    //if the address if an ipv6 address
            port = iperfIpv6Port;
        }
        TransferTask *task = new TransferTask(message.data.transferId, message.data.transferType, message.data.localIp, message.data.remoteHost, message.data.seconds, port, this);
        tasks[message.data.transferId] = task;
        int id = message.data.transferId;
        connect(task, &TransferTask::newStatus, this, &AbstractProtocol::sendMessage);
        connect(task, &AbstractTask::finished,[id, this, task]() {
            tasks.remove(id);
            task->deleteLater();
        });

        tasks[message.data.transferId] = task;
        task->start();

    } else if (type == AbstractMessage::MSGTYPE_TASKCONTROLLER) {

        TaskControllerMessage message;
        message.read(socket);
        if (message.data.command == TaskControllerMessage::COMMAND_STOP) { //a request to stop an exiting transfer task
            int id = message.data.sessionId;

            if (tasks.contains(id)) {

                AbstractTask *task = tasks[id];
                connect(task, &AbstractTask::finished, [id, this]() {
                    tasks.remove(id); //remove the task when it's destroyed
                });

                task->stopTask();
                qDebug() << "Stopping task";
                //we give task 4 seconds to shut down 'gracefully', then if it's still living it's killed forcefully
                QTimer *timer = new QTimer(this);
                connect(timer, &QTimer::timeout, [id, timer, this]() {
                            if (tasks.contains(id)) {
                                tasks[id]->killTask();
                                qDebug() << "Killing the task";
                            }
                            timer->stop();
                            timer->deleteLater();

                        });

                timer->start(2000);
            }
        }

    } else {
        qDebug() << "Unrecongized message type";
        return false;
    }
    return true;
}




