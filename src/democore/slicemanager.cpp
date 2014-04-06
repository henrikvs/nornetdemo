#include "slicemanager.h"
#include <QList>
SliceManager::SliceManager()
{

}

QList<Sliver> SliceManager::getSlivers()
{
    Sliver sliver;
    sliver.IPv4 = "testest";
    sliver.IPv6 = "::1";
    sliver.name = "testslice";
    sliver.sliceName = "srl_nndemo";
    sliver.domain = "tullinloekka.simula.nornet";
    sliver.port = 33555;
    return QList<Sliver>() << sliver;
}
