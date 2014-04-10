#ifndef RELAYPROTOCOL_H
#define RELAYPROTOCOL_H
#include "relayprotocol.h"
#include "abstractprotocol.h"
class RelayProtocol : public AbstractProtocol
{
    Q_OBJECT


public:
    explicit RelayProtocol(QObject *parent = 0);
    virtual void start();
protected:
    virtual bool handleMessage(int type);
signals:

public slots:
};

#endif // RELAYPROTOCOL_H
