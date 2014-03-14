#include "frontend.h"

FrontEnd::FrontEnd()
{
    QObject::connect(&client, &Client::connectionError, this, &FrontEnd::failedToConnect);
    QList<Sliver> slivers = sliceManager.getSlivers();
    foreach (Sliver sliver, slivers) {
        client.connectToNode(sliver.IPv6, sliver.port);
    }
}

void FrontEnd::failedToConnect(QString host, int port)
{
    qDebug() << "failed to connect to: " << host << port;
}

void FrontEnd::connected(QString host, int port)
{
    qDebug() << "Connection success: " << host << port;
}
