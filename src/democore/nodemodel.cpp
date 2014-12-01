#include "nodemodel.h"

NodeModel::NodeModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    return sliversList.size();
}

int NodeModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
       Sliver *sliver = sliversList[index.row()];
        switch (index.column()) {
            case NAME_INDEX:
                return sliver->hostName;
                break;

            case STATUS_INDEX:
                return sliver->getStatusString();
                break;
            case IP_INDEX:
                return sliver->IPv6;
                break;
            default:
                return "unknown";
                break;
        }
    }
    return QVariant();
}

void NodeModel::addNode(Sliver *node)
{
    if (!slivers.contains(node->hostName)) {

        slivers.insert(node->hostName, node);
        sliversList << node;
        beginInsertRows(QModelIndex(), sliversList.size(), sliversList.size());
        insertRow(sliversList.size());
        endInsertRows();
    }
}

Sliver* NodeModel::getNode(QString name)
{
    return slivers[name];
}

void NodeModel::setNodes(QList<Sliver *> nodes)
{

   foreach (Sliver *sliver, nodes) {
       if (!slivers.contains(sliver->hostName)) {
            sliversList << sliver;
            slivers.insert(sliver->hostName, sliver);
       }
   }
}

QList<Sliver *> NodeModel::getNodes()
{
    return sliversList;
}

void NodeModel::removeNode(QString name)
{
    int index = findIndex(name);
    if (index < 0) {
        qDebug() << "Name not found";
        return;
    }
    qDebug() << "Deleting " << name << " index: " << index;
    beginRemoveRows(QModelIndex(), index, index);
    Sliver *sliver = slivers.take(name);
    int removed = sliversList.removeAll(sliver);
    qDebug() << "Removed: " << removed;
    delete sliver;
    endRemoveRows();
}

void NodeModel::editNode(QString nodeName, Sliver node)
{
    if (slivers.contains(node.name)) {
        return;
    }
   Sliver *sliver = slivers[nodeName];
   *sliver = node;
   emit dataChanged(createIndex(0,0), createIndex(sliversList.size(), 2)); //improve this
}

bool NodeModel::contains(QString name)
{
    return slivers.contains(name);
}

int NodeModel::findIndex(QString name)
{
    int i = 0;
    foreach (Sliver *sliver, sliversList) {
        if (sliver->hostName == name) {
            return i;
        }
        i++;
    }
    return -1;
}

void NodeModel::onStatusChanged(QString name)
{
   int index = findIndex(name);
   if (index < 0) {
       return;
   }
   emit dataChanged(createIndex(index,0), createIndex(index, sliversList.size())); //improve this
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
   return Qt::ItemIsSelectable | Qt::ItemIsEnabled;


}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case NAME_INDEX:
                return "Name";
                break;

            case STATUS_INDEX:
                return "Status";
                break;
            case IP_INDEX:
                return "IP";
                break;
            default:
                return "Error";
                break;
        }
    }
    return QVariant();
}

