#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>

class Node;

class Communicator : public QObject
{
    Q_OBJECT

public:
    void findLocation(Node &node);

    static Communicator& getInstance();

signals:
    void foundLocation(QString location);

private:
    Communicator(Communicator const&);
    void operator=(Communicator const&);
    Communicator() {}
};

#endif // COMMUNICATOR_H
