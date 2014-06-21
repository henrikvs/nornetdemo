#ifndef SLIVER_H
#define SLIVER_H
#include "nodeinfomessage.h"
#include <QString>
class Sliver
{
public:
    const static int STATUS_UPDATING = 1, STATUS_CONNECTED = 2, STATUS_CONNECTING=5, STATUS_OFFLINE = 3, STATUS_INSTALLING = 4;
    Sliver();
    QString name;
    QString IPv4;
    QString IPv6;
    QString sliceName;
    QString hostName;
    NodeInfoMessage infoMessage;
    int status;
    int port;
};

#endif // SLIVER_H
