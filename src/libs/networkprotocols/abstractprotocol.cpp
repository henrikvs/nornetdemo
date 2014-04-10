#include "nodeprotocol.h"
#include "pingreply.h"
#include "pingrequest.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "nodeinfomessage.h"
#include "nodeinforequest.h"
#include "pingtask.h"
#include "infotask.h"


AbstractProtocol::AbstractProtocol(QObject *parent) : QObject(parent), headerRead(false)
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
    qDebug() << "Sending Total size 2: " << totalData.size();
    qDebug() << "Bytes to write: " << socket->bytesToWrite();

    int ret = socket->write(totalData);
    if (ret != totalData.size()) {
        qDebug() << "Error!! Ret = " << ret;
        qDebug() << socket->errorString();
    }
    socket->flush();

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
    return false;

}



