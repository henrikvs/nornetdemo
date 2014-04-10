#include "requestmessage.h"

#include <QDataStream>
#include <QIODevice>

void RequestMessage::send(QTcpSocket &socket)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint16) 0; //save for the size
    out << message;
    out.device() -> seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket.write(block);
}


bool RequestMessage::receive(QTcpSocket &socket)
{

    if (socket.bytesAvailable() < SIZE_MSGSIZE) {
        return false;
    }

    quint16 size;
    //socket.peek(&size, (int)sizeof(quint16));
    //if ()
        //in >> messageSize;
    //}
    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_5_0);

}
