#ifndef SLIVER_H
#define SLIVER_H
#include "nodeinfomessage.h"
#include <QString>
class Sliver
{
public:
    const static int STATUS_UPDATING = 1, STATUS_CONNECTED = 2, STATUS_CONNECTING=5, STATUS_OFFLINE = 3, STATUS_INSTALLING = 4, STATUS_READY = 6;
    Sliver();
    QString IPv4;
    QString IPv6;
    QString hostName;
    NodeInfoMessage infoMessage;
    int port;
    bool enabled;
    void setStatus(int status);
    QString getStatusString();
    int getStatus();
    bool isActive();
    void setActive(bool active);
private:
    bool active;
    int status;
};

#endif // SLIVER_H
