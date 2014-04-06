#ifndef NODEPROG_H
#define NODEPROG_H
#include <QList>
#include <QObject>
#include <QDebug>
#include "networkentity.h"
#include "abstractprotocol.h"
#include "nodeprotocol.h"
class NodeProg : public NetworkEntity
{
    Q_OBJECT
public:
    NodeProg();
protected:
    virtual AbstractProtocol *createProtocol(int type, MyQTcpSocket *socket);
private slots:
    void failedToConnect(QString host, int port);
    void connected(QString host, int port);
    void exitProgram(int exitValue);
};

#endif // NODEPROG_H
