#include "demoprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
#include "pingrequest.h"
#include "transferrequestmessage.h"
#include "transferstatusmessage.h"
#include <QTimer>
DemoProtocol::DemoProtocol(QObject *parent) : AbstractProtocol(parent)
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

void DemoProtocol::sendPingRequest(int sessionId, QString remoteIp, QString localIp) {
    static int nextId = 0;
    nextId++;
    PingRequest message(sessionId, remoteIp, localIp);
    sendMessage(message);
}

void DemoProtocol::sendTransferRequest(int id, QString host, QString localIp, int transferType, int seconds)
{
    //int id = TransferRequestMessage::nextId();

    TransferRequestMessage message(id, transferType, host, localIp, seconds);
    sendMessage(message);
}



void DemoProtocol::sendKeepAlive() {
    StringMessage message("keepalive");
    sendMessage(message);
}

void DemoProtocol::start()
{
    qDebug() << "Starting protocol";
    sendDemoStatus();
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    timer->start(10000);*/
}

void DemoProtocol::cleanUp()
{

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
            emit nodeUpdating(socket);
        } else if (message.data.status == NodeStatusMessage::EXITING) {
            emit nodeExiting(socket);
        } else if (message.data.status == NodeStatusMessage::RESTARTING) {
            emit nodeRestarting(socket);
        }
    } else if (type == MSGTYPE_NODEINFO) {
        NodeInfoMessage message;
        message.read(socket);
        //qDebug() << "Name: " << message.data.name;
        qDebug() << "Listening port: " << message.data.listeningPort;
        foreach (NetworkInterface interface, message.data.interfaces) {
            qDebug() << "Interface: " << interface.interfaceName << interface.addresses;
        }
        emit newNodeInfo(message, socket);

    } else if (type == MSGTYPE_PINGREPLY) {
        PingReply message;
        message.read(socket);
        emit newPingReply(message, socket);
        qDebug() << "Ping:" << message.data.ms;
    } else if (type == MSGTYPE_TRANSFERSTATUS) {
        TransferStatusMessage message;
        message.read(socket);
        emit newTransferStatus(message, socket);
    } else {
        return false;
    }
    return true;
}
