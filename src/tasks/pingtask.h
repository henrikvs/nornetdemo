#ifndef PINGTASK_H
#define PINGTASK_H

#include <QObject>

class PingTask : public QObject
{
    Q_OBJECT
public:
    explicit PingTask(int taskNumber, QObject *parent = 0);
    int taskNumber;
signals:
    void newPing();
    void pingFinished();
public slots:

};

#endif // PINGTASK_H
