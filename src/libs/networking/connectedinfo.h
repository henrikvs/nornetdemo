#include "networklibglobal.h"

#include <QString>
#ifndef CONNECTEDINFO_H
#define CONNECTEDINFO_H

class NETWORKLIB_EXPORT ConnectionInfo
{
public:
    ConnectionInfo();
    ConnectionInfo(QString address, int port, bool ipv4, int type, QString hostname = QString(), QString username = QString());
    bool isNode();
    void setIsNode();
    void setIsFrontEnd();
    bool isIpv4();
    QString getAddress();
    int getPort();
    QString getHostName();
    QString getUsername();
    int getType();
    void setType(int type);
private:
    bool node;
    QString address;
    int port;
    bool ipv4;
    QString hostname;
    QString username;
    int type;


};

#endif // CONNECTEDINFO_H
