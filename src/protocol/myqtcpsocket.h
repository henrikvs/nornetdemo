#ifndef MYQTCPSOCKET_H
#define MYQTCPSOCKET_H

#include <QTcpSocket>

class MyQTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyQTcpSocket(QObject *parent = 0);
    void addSize(int size);
    bool sizeRead();
    void resetSize();
private:
    int size;


signals:

public slots:

};

#endif // MYQTCPSOCKET_H
