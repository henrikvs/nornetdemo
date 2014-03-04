#include "connectedinfo.h"

ConnectedInfo::ConnectedInfo()
{

}

bool ConnectedInfo::isNode()
{
    return node;
}

void ConnectedInfo::setIsNode()
{
    node = true;
}

void ConnectedInfo::setIsFrontEnd()
{
    node = false;
}
