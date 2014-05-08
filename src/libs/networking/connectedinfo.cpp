#include "connectedinfo.h"

ConnectionInfo::ConnectionInfo()
{

}

ConnectionInfo::ConnectionInfo(QString address, int port, bool ipv4, int type, QString hostname, QString username) :
    address(address), port(port), ipv4(ipv4), hostname(hostname), type(type), username(username)
{


}

bool ConnectionInfo::isNode()
{
    return node;
}

void ConnectionInfo::setIsNode()
{
    node = true;
}

void ConnectionInfo::setIsFrontEnd()
{
    node = false;
}

bool ConnectionInfo::isIpv4()
{
    return ipv4;
}

QString ConnectionInfo::getAddress()
{
    return address;
}

int ConnectionInfo::getPort()
{
    return port;
}

QString ConnectionInfo::getHostName()
{
    return hostname;
}

QString ConnectionInfo::getUsername()
{
    return username;
}

int ConnectionInfo::getType()
{
    return type;
}

void ConnectionInfo::setType(int type)
{
    this->type = type;
}
