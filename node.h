#ifndef NODE_H
#define NODE_H

#include "communicator.h"
#include <QObject>
#include <QList>
#include "connection.h"

class Node : public QObject
{
    Q_OBJECT

public:
    Node();
    QString location;
    void findLocation();
    QString sliverName;

private:
    QList<Connection> connections;

private slots:
    void setLocation(QString location);


};

#endif // NODE_H
