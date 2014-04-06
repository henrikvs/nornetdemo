#ifndef MYQTCPSOCKET_H
#define MYQTCPSOCKET_H

#include <QTcpSocket>
#include "connectedinfo.h"
class AbstractProtocol;

class MyQTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyQTcpSocket(QObject *parent = 0);
    void addSize(int size);
    void setId(int id);
    void setProtocol(AbstractProtocol *protocol);
    void setConnectionInfo(ConnectionInfo info);
    ConnectionInfo getConnectionInfo();
    AbstractProtocol* getProtocol();
    int getId();
    bool sizeRead();
    void resetSize();
private:
    int size;
    ConnectionInfo info;
    int id;
    AbstractProtocol *protocol;


signals:

public slots:

};

#endif // MYQTCPSOCKET_H
