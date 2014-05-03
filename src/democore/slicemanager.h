#ifndef SLICEMANAGER_H
#define SLICEMANAGER_H

#include "sliver.h"
#include <QList>

class SliceManager
{
public:
    SliceManager();
    QList<Sliver*> getSlivers();
    QHash<QString, Sliver*> slivers;
    void createSliver(QString hostname, QString username);
    void removeSliver(QString name);
    bool sliverExists(QString hostname);
    void readSliversFromFile(QString file = "slivers.data");
    void writeSliversToFile(QString file = "slivers.data");
};

#endif // SLICEMANAGER_H
