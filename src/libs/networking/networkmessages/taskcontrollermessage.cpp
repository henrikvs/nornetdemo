#include "taskcontrollermessage.h"
#include <QDataStream>

TaskControllerMessage::TaskControllerMessage()
{

}

TaskControllerMessage::TaskControllerMessage(int sessionId, int command)
{
	this->data.sessionId=sessionId;
	this->data.command=command;
}

void TaskControllerMessage::serialize(QByteArray *block) const
{
    QDataStream out(block, QIODevice::Append);
    out.setVersion(QDataStream::Qt_5_0);
    out << data.sessionId << data.command;
}

void TaskControllerMessage::read(MyQTcpSocket *socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_0);
    in >> data.sessionId >> data.command;
}

char TaskControllerMessage::getType() const
{
    return MSGTYPE_TASKCONTROLLER;
}

