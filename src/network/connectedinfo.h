#include <QString>
#ifndef CONNECTEDINFO_H
#define CONNECTEDINFO_H

class ConnectionInfo
{
public:
    ConnectionInfo();
    ConnectionInfo(QString address, int port, bool ipv4);
    bool isNode();
    void setIsNode();
    void setIsFrontEnd();
    bool isIpv4();
    QString getAddress();
    int getPort();
private:
    bool node;
    QString address;
    int port;
    bool ipv4;


};

#endif // CONNECTEDINFO_H
