#include "nodeinforequest.h"
#include "nodeprotocol.h"
NodeInfoRequest::NodeInfoRequest()
{
}

void NodeInfoRequest::serialize(QByteArray *block)  const
{

}

void NodeInfoRequest::read(MyQTcpSocket *socket)
{

}

char NodeInfoRequest::getType() const
{
return AbstractProtocol::MSGTYPE_NODEINFOREQUEST;
}
