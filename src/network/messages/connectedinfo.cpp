#include "connectedinfo.h"

ConnectionInfo::ConnectionInfo()
{

}

ConnectionInfo::ConnectionInfo(QString address, int port, bool ipv4) : address(address), port(port), ipv4(ipv4)
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
