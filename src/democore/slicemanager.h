#ifndef SLICEMANAGER_H
#define SLICEMANAGER_H

#include "sliver.h"
#include "nodemodel.h"
#include <QList>

class SliceManager : public QObject
{
    Q_OBJECT
public:
    SliceManager();
    QList<Sliver*> getSlivers();
    int sliverCount();
    void createSliver(QString hostname, int port);
    void removeSliver(QString name);
    bool sliverExists(QString hostname);
    void readSliversFromFile(QString file = "slivers.data");
    void writeSliversToFile(QString file = "slivers.data");
    Sliver * getSliver(QString siteName);
    NodeModel* getModel();
    void editSliver(QString nodeName, Sliver newNode);
    void setStatus(QString nodeName, Sliver newNode);
private:
    NodeModel model;
signals:
    void newNodeUpdate(QString originalName, Sliver sliver);
};

#endif // SLICEMANAGER_H
