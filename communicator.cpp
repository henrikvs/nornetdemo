#include "communicator.h"

/*void Communicator::findLocation(Node node)
{
    QString sliverName = node.sliverName;
    QString sshCommand = "dig loc " + sliverName;
    //QString res = do ssh command;
    //parse location from res

    //do ssh dig magic
    emit foundLocation("res");
}*/

Communicator &Communicator::getInstance()
{
    static Communicator instance;
    return instance;
}
