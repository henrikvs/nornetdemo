#include "nodeprotocol.h"
#include "pingreply.h"
#include "pingrequest.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "nodeinfomessage.h"
#include "nodeinforequest.h"
#include "pingtask.h"
#include "infotask.h"


AbstractProtocol::AbstractProtocol(int id, QObject *parent) : QObject(parent), headerRead(false), id(id)
{

}


void AbstractProtocol::newData()
{
    while (1) {


        qDebug() << "new data total size: " << socket->bytesAvailable();
        if (!headerRead) {
            if (socket->bytesAvailable() < SIZE_HEADER) {
                return;
            } else {
                header.read(socket);

                qDebug() << "size: " << header.data.size;
                qDebug() << "Type: " << header.data.type;
            }
        }

        if (socket->bytesAvailable() < header.data.size) {
            headerRead = true;
            return;
        } else {
            headerRead = false;
        }
        int type = header.data.type;


        handleMessage(type);
    }

}

void AbstractProtocol::sendPingReply(QString ms) {
    PingReply reply(ms);
    sendMessage(reply);
}

void AbstractProtocol::sendPingRequest(QString remoteIp) {
    PingRequest message(remoteIp);
    sendMessage(message);
}

void AbstractProtocol::sendNodeInfo(QStringList isps, QString port) {
    NodeInfoMessage message;
    //message.data.ISPConnections = isps;
    message.data.listeningPort = port;
    sendMessage(message);
}

void AbstractProtocol::sendNodeInfoRequest() {
    NodeInfoRequest request;
    sendMessage(request);
}

void AbstractProtocol::sendMessage(const AbstractMessage &message)
{
    QByteArray totalData;
    QByteArray messageData;
    MessageHeader header;

    message.serialize(&messageData);
    header.data.size = messageData.size();
    header.data.type = message.getType();
    header.serialize(&totalData);
    totalData.append(messageData);
    qDebug() << "Sending Total size: " << totalData.size();
    socket->write(totalData);

}

int AbstractProtocol::getId()
{
    return id;
}


void AbstractProtocol::setSocket(MyQTcpSocket *socket)
{
    this->socket = socket;
}

void AbstractProtocol::setType(qint32 type)
{
    this->type = type;
}

qint32 AbstractProtocol::getType()
{
    return type;
}


/**
 * @brief NodeProtocol::handleMessage
 * @param type
 * the id of the message that needs to be read
 * @return
 * returns true if the message was handled
 */

bool AbstractProtocol::handleMessage(int type)
{
    bool messageHandled = true;
    if (type == MSGTYPE_PINGREQUEST) {
        PingRequest message;
        message.read(socket);
        qDebug() << "Remote ip: " << message.data.remoteIp;


        PingTask *task = new PingTask(message.data.remoteIp);
        connect(task, &PingTask::newPing, this, &AbstractProtocol::sendMessage);
        connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
        task->start();

    } else if (type == MSGTYPE_PINGREPLY) {
        PingReply message;
        message.read(socket);
        emit newPingReply(message, getId());
        qDebug() << "Ping:" << message.data.ms;
    } else if (type == MSGTYPE_COMMANDREQUEST) {
        PingRequest message;
        message.read(socket);
    } else if (type == MSGTYPE_COMMANDREPLY) {
        PingRequest message;
        message.read(socket);
    } else if (type == MSGTYPE_NODEINFOREQUEST) {
        PingRequest message;
        message.read(socket);
        InfoTask *task = new InfoTask(this);
        connect(task, &InfoTask::newInfoMessage, this, &AbstractProtocol::sendMessage);
        connect(task, SIGNAL(finished()), task, SLOT(deleteLater()));
        task->start();

    } else if (type == MSGTYPE_NODEINFO) {
        NodeInfoMessage message;
        message.read(socket);
        qDebug() << "Listening port: " << message.data.listeningPort;
        foreach (Interface interface, message.data.interfaces) {
            qDebug() << "Interface: " << interface.interfaceName << interface.addresses;
        }
        emit newNodeInfo(message, getId());
    } else {
        messageHandled = false;
    }
    return messageHandled;

}



