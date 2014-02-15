#include "node.h"

Node::Node()
{
}

void Node::findLocation()
{
     Communicator &comm = Communicator::getInstance();
     connect(&comm, SIGNAL(foundLocation(QString)), this, SLOT(setLocation(QString)));
}

void Node::setLocation(QString location)
{
    this->location = location;
}
