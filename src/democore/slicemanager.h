#ifndef SLICEMANAGER_H
#define SLICEMANAGER_H

#include "sliver.h"
#include <QList>

class SliceManager
{
public:
    SliceManager();
    QList<Sliver> getSlivers();

};

#endif // SLICEMANAGER_H
