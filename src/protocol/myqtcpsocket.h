#ifndef MYQTCPSOCKET_H
#define MYQTCPSOCKET_H

#include <QTcpSocket>
#include "connectedinfo.h"

class MyQTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyQTcpSocket(QObject *parent = 0);
    void addSize(int size);
    bool sizeRead();
    void resetSize();
    ConnectedInfo getConnectedInfo();
private:
    int size;
    ConnectedInfo info;


signals:

public slots:

};

#endif // MYQTCPSOCKET_H
