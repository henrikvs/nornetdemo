#include "slicemanager.h"
#include <QList>
SliceManager::SliceManager()
{
    Sliver *sliver = new Sliver;
    sliver->IPv4 = "testest";
    sliver->IPv6 = QString();
    sliver->name = "tullinloekka";
    sliver->sliceName = "srl_nndemo";
    sliver->domain = "tullinloekka.simula.nornet";
    sliver->port = 33555;
    slivers << sliver;
}

QList<Sliver*> SliceManager::getSlivers()
{
    return slivers;
}
