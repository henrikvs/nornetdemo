#ifndef SLIVER_H
#define SLIVER_H
#include <QString>
class Sliver
{
public:
    const static int STATUS_UPDATING = 1, STATUS_CONNECTED = 2, STATUS_OFFLINE = 3;
    Sliver();
    QString name;
    QString IPv4;
    QString IPv6;
    QString sliceName;
    QString domain;
    int status;
    int port;
};

#endif // SLIVER_H
