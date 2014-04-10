#include <QString>
#ifndef CONNECTEDINFO_H
#define CONNECTEDINFO_H

class ConnectionInfo
{
public:
    ConnectionInfo();
    ConnectionInfo(QString address, int port, bool ipv4, QString name = QString());
    bool isNode();
    void setIsNode();
    void setIsFrontEnd();
    bool isIpv4();
    QString getAddress();
    int getPort();
    QString getName();
private:
    bool node;
    QString address;
    int port;
    bool ipv4;
    QString name;


};

#endif // CONNECTEDINFO_H
