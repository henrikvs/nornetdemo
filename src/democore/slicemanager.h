#ifndef SLICEMANAGER_H
#define SLICEMANAGER_H

#include "sliver.h"
#include <QList>

class SliceManager
{
public:
    SliceManager();
    QList<Sliver*> getSlivers();
    QList<Sliver*> slivers;
};

#endif // SLICEMANAGER_H
