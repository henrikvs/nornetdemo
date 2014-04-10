#ifndef DEMOPROTOCOL_H
#define DEMOPROTOCOL_H
#include "abstractprotocol.h"
class DemoProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit DemoProtocol(int id, QObject *parent = 0);
    void sendDemoStatus();
    void sendExit();
    virtual void start();
protected:
    virtual bool handleMessage(int type);
signals:
    void nodeRestarting(int nodeId);
    void nodeUpdating(int nodeId);
    void nodeExiting(int nodeId);
};

#endif // DEMOPROTOCOL_H
