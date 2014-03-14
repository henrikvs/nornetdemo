#ifndef FRONTEND_H
#define FRONTEND_H
#include <QList>
#include "slicemanager.h"
#include "sliver.h"
#include "client.h"
class FrontEnd : public QObject
{
    Q_OBJECT
public:
    FrontEnd();
private:
    SliceManager sliceManager;
    Client client;
private slots:
    void failedToConnect(QString host, int port);
    void connected(QString host, int port);
};

#endif // FRONTEND_H
