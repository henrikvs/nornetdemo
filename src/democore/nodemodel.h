#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractTableModel>
#include "sliver.h"

class NodeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NodeModel(QObject *parent = 0);
    ~NodeModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    void addNode(Sliver *node);
    Sliver* getNode(QString name);
    Sliver* getNode(int index);
    QVariant data(const QModelIndex &index, int role) const;
    void setNodes(QList <Sliver*> nodes);
    QList<Sliver*> getNodes();
    void removeNode(QString name);
    void editNode(QString nodeName, Sliver node);
    bool contains(QString name);

private:
    QHash<QString, Sliver*> slivers;
    QList<Sliver*> sliversList;
    const static int NAME_INDEX = 0;
    const static int STATUS_INDEX = 1;
    const static int IP_INDEX = 2;
    int findIndex(QString name);

    QList<Sliver*> inactiveSlivers;
signals:


public slots:

private slots:
    void onStatusChanged(QString name);

       // QAbstractItemModel interface

        // QAbstractItemModel interface
        public:
        bool setData(const QModelIndex &index, const QVariant &value, int role);

        // QAbstractItemModel interface

        // QAbstractItemModel interface
public:
        Qt::ItemFlags flags(const QModelIndex &index) const;

        // QAbstractItemModel interface
public:
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        // QAbstractItemModel interface
public:
        bool removeRows(int row, int count, const QModelIndex &parent);
};

#endif // NODEMODEL_H
