#ifndef COMMANDMESSAGE_H
#define COMMANDMESSAGE_H

#include <QByteArray>
#include "abstractprotocol.h"
struct StringMessageStruct {

};

class CommandMessage : public AbstractMessage
{
public:
    CommandMessage();
    CommandMessage(QString command);
};

#endif // COMMANDMESSAGE_H
