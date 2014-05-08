#include "nodeinforequest.h"
#include "abstractprotocol.h"
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
return MSGTYPE_NODEINFOREQUEST;
}
