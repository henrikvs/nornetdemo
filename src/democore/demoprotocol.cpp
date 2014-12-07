#include "demoprotocol.h"
#include "demostatusmessage.h"
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodestatusmessage.h"
#include "stringmessage.h"
#include "nodeinforequest.h"
#include "pingrequest.h"
#include "transferrequestmessage.h"
#include "transferstatusmessage.h"
#include "taskcontrollermessage.h"
#include <QTimer>

/** \class DemoProtocol
 * \brief This class contains the communication protocol for the NorNet Demo progam. It assumes a node and a NodeProtocol
 * on the other side of the socket.
 */

DemoProtocol::DemoProtocol(QObject *parent) : AbstractProtocol(parent)
{

}

/**
 * @brief Sends a message with the status of the NorNet demo program. This is currently only the version of the
 * communication protocol.
 */
void DemoProtocol::sendDemoStatus()
{
    qDebug() << "Sending demo status";
    DemoStatusMessage message(NetworkEntity::VERSION);
    sendMessage(message);
}

/**
 * @brief Sends a message telling the node conneced to shut down.
 */
void DemoProtocol::sendExit()
{
    StringMessage message("exit");
    sendMessage(message);

}

/**
 * @brief Sends a request to receive information about the connected node.
 */
void DemoProtocol::sendNodeInfoRequest() {
    NodeInfoRequest request;
    sendMessage(request);
}

/**
 * @brief Sends a ping request through the socket associated with this protocol instance.
 * @param sessionId A unique sessionId.
 * @param remoteIp The IP (v6 or v4) to connect to.
 * @param localIp The local IP(v6 or v4) to bind to.
 * @param seconds The duration of the ping in seconds
 */
void DemoProtocol::sendPingRequest(int sessionId, QString remoteIp, QString localIp, int seconds) {
    PingRequest message(sessionId, remoteIp, localIp, seconds);
    sendMessage(message);
}

/**
 * @brief Sends a transfer request through the socket associated with thi protocol instance.
 * @param id A unique sessionId.
 * @param host IPv4 or IPv6 of the host to start the transfer test with.
 * @param localIp The local IPv4 or IPv6 to bind to.
 * @param transferType The type of transfer. See the static consts of TransferRequestMessage.
 * @param seconds Duration of the experiment.
 */
void DemoProtocol::sendTransferRequest(int id, QString host, QString localIp, int transferType, int seconds)
{
    //int id = TransferRequestMessage::nextId();

    TransferRequestMessage message(id, transferType, host, localIp, seconds);
    sendMessage(message);
}

/**
 * @brief Asks the connected node to stop a task(ping or tcp) with the sessionId specified.
 * @param sessionId The id of the task to stop.
 */
void DemoProtocol::sendStopTask(int sessionId)
{
    TaskControllerMessage message(sessionId, TaskControllerMessage::COMMAND_STOP);
    sendMessage(message);
}

/**
 * @brief  Currently not used.
 */
void DemoProtocol::sendKeepAlive() {
    StringMessage message("keepalive");
    sendMessage(message);
}

/**
 * @brief Initialization done upon starting the protocol instance.
 */
void DemoProtocol::start()
{
    AbstractProtocol::start();
    qDebug() << "Starting protocol";
    sendDemoStatus();
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendKeepAlive()));
    timer->start(10000);*/
}

void DemoProtocol::cleanUp()
{

}


/**
 * @brief Reads and handles responses to incoming messages.
 * @param type The message type. Used to read the parse the data of the socket correctly.
 * @return Returns true if the message was handled, otherwise false.
 */
bool DemoProtocol::handleMessage(int type)
{
    if (AbstractProtocol::handleMessage(type)) {
        return true;
    } else if (type == AbstractMessage::MSGTYPE_NODESTATUSMESSAGE) {
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
    } else if (type == AbstractMessage::MSGTYPE_NODEINFO) {
        NodeInfoMessage message;
        message.read(socket);
        //qDebug() << "Name: " << message.data.name;
        qDebug() << "Listening port: " << message.data.listeningPort;
        foreach (NetworkInterface interface, message.data.interfaces) {
            qDebug() << "Interface: " << interface.interfaceName << interface.addresses;
        }
        emit newNodeInfo(message, socket);

    } else if (type == AbstractMessage::MSGTYPE_PINGREPLY) {
        PingReply message;
        message.read(socket);
        emit newPingReply(message, socket);
        qDebug() << "Ping:" << message.data.ms;
    } else if (type == AbstractMessage::MSGTYPE_TRANSFERSTATUS) {
        TransferStatusMessage message;
        message.read(socket);
        emit newTransferStatus(message, socket);

    } else if (type == AbstractMessage::MSGTYPE_STRINGMESSAGE) {
        StringMessage message;
        message.read(socket);
        QString command = message.data.text;
        if (command == "heartbeat") {
            qDebug() << "Received new heartbeat";
        }
    } else {
        return false;
    }
    return true;
}
