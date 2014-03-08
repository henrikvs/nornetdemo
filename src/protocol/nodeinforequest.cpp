#include "nodeinforequest.h"
#include "nodeprotocol.h"
NodeInfoRequest::NodeInfoRequest(QObject *parent) : AbstractMessage(parent)
{
}

void NodeInfoRequest::serialize(QByteArray *block)
{

}

void NodeInfoRequest::read(MyQTcpSocket *socket)
{

}

char NodeInfoRequest::getType()
{
return NodeProtocol::MSGTYPE_NODEINFOREQUEST;
}
