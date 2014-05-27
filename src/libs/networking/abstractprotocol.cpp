#include "abstractprotocol.h"
#include "abstractmessage.h"
#include "messageheader.h"
#include "stringmessage.h"
#include <QTimer>

/**
 * @brief AbstractProtocol::AbstractProtocol
 * @param The QObject parent, used for destruction
 */
AbstractProtocol::AbstractProtocol(QObject *parent) : QObject(parent), headerRead(false)
{
    QTimer *timer = new QTimer(this);
    //Send out a message every now and then, to keep the tcp connection alive
    connect(timer, &QTimer::timeout, [this]() {
       sendHeartbeat();
    });
    timer->start(30000);
    isRelay = false;
    active = true;
}


/**
 * @brief Called when new data is received on the associated socket, if sufficient data is available, it will call the virtual method handleMessage(int)
 */
void AbstractProtocol::newData()
{
    if (!active) {
        return;
    }

    while (1) {


        qDebug() << "new data total size: " << socket->bytesAvailable();
        if (!headerRead) {
            if (socket->bytesAvailable() < SIZE_HEADER) {
                return;
            } else {
                header.read(socket);
                headerRead = true;
                qDebug() << "size: " << header.data.size;
                qDebug() << "Type: " << header.data.type;
            }
        }


        if (socket->bytesAvailable() < header.data.size) {
            return;
        }

        headerRead = false;
        int type = header.data.type;
        handleMessage(type);
    }

}
/*
void AbstractProtocol::sendPingReply(QString ms) {
    PingReply reply(ms);
    sendMessage(reply);
}*/

void AbstractProtocol::sendMessage(const AbstractMessage &message)
{
    QByteArray totalData;
    QByteArray messageData;
    MessageHeader header;

    static int nextSeq = 0;
    nextSeq++;
    message.serialize(&messageData);
    header.data.size = messageData.size();
    header.data.type = message.getType();
    header.data.seqNum = nextSeq;
    header.serialize(&totalData);
    totalData.append(messageData);
    qDebug() << "Sending Total size: " << totalData.size();

    int written = 0;
    int totalSize = totalData.size();
    while (written < totalSize) {
        int ret = socket->write(totalData.data() + written, totalSize - written);
        if (ret == -1) {
            qDebug() << "write error";
            return;
        }
        written+=ret;
    }
}

void AbstractProtocol::sendHeartbeat()
{
    StringMessage message("heartbeat");
    qDebug() << "Sent heartbeat";
    sendMessage(message);
}

void AbstractProtocol::setName(QString name)
{
    this->name = name;
}

QString AbstractProtocol::getName()
{
    return name;
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



