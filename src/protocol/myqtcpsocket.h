#ifndef MYQTCPSOCKET_H
#define MYQTCPSOCKET_H

#include <QTcpSocket>
#include "connectedinfo.h"
class NodeProtocol;

class MyQTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyQTcpSocket(QObject *parent = 0);
    void addSize(int size);
    void setId(int id);
    void setProtocol(NodeProtocol *protocol);
    NodeProtocol* getNodeProtocol();
    int getId();
    bool sizeRead();
    void resetSize();
    ConnectedInfo getConnectedInfo();
private:
    int size;
    ConnectedInfo info;
    int id;
    NodeProtocol *protocol;


signals:

public slots:

};

#endif // MYQTCPSOCKET_H
