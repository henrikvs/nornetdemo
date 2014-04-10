#include "protocolheader.h"

struct ProtocolHeaderStruct {
    int protocolType;
};

ProtocolHeader::ProtocolHeader(QObject *parent) :
    QObject(parent)
{
}
