#include "nodeprotocol.h"
#include "pingreply.h"
#include "pingrequest.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "nodeinfomessage.h"


NodeProtocol::NodeProtocol(MyQTcpSocket *socket, QObject *parent) : QObject(parent), socket(socket), headerRead(false)
{

}


void NodeProtocol::newData()
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

        if (type == MSGTYPE_PINGREQUEST) {
            PingRequest message;
            message.read(socket);
            qDebug() << "Remote ip: " << message.data.remoteIp;
            //QString ping =
            PingReply reply("123");
            sendMessage(reply);

        } else if (type == MSGTYPE_PINGREPLY) {
            PingReply message;
            message.read(socket);

            qDebug() << "Ping:" << message.data.ms;
        } else if (type == MSGTYPE_COMMANDREQUEST) {
            PingRequest message;
            message.read(socket);
        } else if (type == MSGTYPE_COMMANDREPLY) {
            PingRequest message;
            message.read(socket);
        } else if (type == MSGTYPE_NODEINFO) {
            NodeInfoMessage message;
            message.read(socket);
            qDebug() << "ISPs: " << message.data.ISPConnections;
            qDebug() << "Port: " << message.data.listeningPort;
        }
    }

}

void NodeProtocol::sendPingReply(QString ms) {
    PingReply reply(ms);
    sendMessage(reply);
}

void NodeProtocol::sendPingRequest(QString remoteIp) {
    PingRequest message(remoteIp);
    sendMessage(message);
}

void NodeProtocol::sendNodeInfo(QStringList isps, QString port) {
    NodeInfoMessage message;
    message.data.ISPConnections = isps;
    message.data.listeningPort = port;
    sendMessage(message);
}

void NodeProtocol::sendMessage(AbstractMessage &message)
{
    QByteArray totalData;
    QByteArray messageData;
    MessageHeader header;

    message.serialize(&messageData);
    header.data.size = messageData.size();
    header.data.type = message.getType();
    header.serialize(&totalData);
    totalData.append(messageData);
    qDebug() << "Total size: " << totalData.size();
    socket->write(totalData);

}


