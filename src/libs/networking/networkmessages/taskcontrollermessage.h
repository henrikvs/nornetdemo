#ifndef TASKCONTROLLERMESSAGE_H
#define TASKCONTROLLERMESSAGE_H
#include <QByteArray>
#include "abstractprotocol.h"

struct TaskControllerMessageStruct {
	int sessionId;
	int command;
};

class TaskControllerMessage : public AbstractMessage
{
public:
    static const int COMMAND_STOP = 3;
    TaskControllerMessage();
    TaskControllerMessage(int sessionId, int command);
    void serialize(QByteArray *block)  const;
    void read(MyQTcpSocket *socket);
    char getType() const;
    TaskControllerMessageStruct data;
};

#endif // TASKCONTROLLERMESSAGE_H
