#include "sliver.h"

Sliver::Sliver()
{
    status = STATUS_OFFLINE;
}

void Sliver::setStatus(int status)
{
    this->status = status;
}

QString Sliver::getStatusString()
{
    switch (status) {
    case STATUS_OFFLINE:
        return "offline";
        break;
    case STATUS_CONNECTED:
        return "connected";
        break;
    case STATUS_CONNECTING:
        return "connecting";
        break;
    case STATUS_INSTALLING:
        return "installing";
        break;
    case STATUS_UPDATING:
        return "updating";
        break;
    default:
        return "unknown";
        break;
    }
}

int Sliver::getStatus()
{
    return status;
}
