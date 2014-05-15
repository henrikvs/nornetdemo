#ifndef TRANSFERTASK_H
#define TRANSFERTASK_H
#include <QObject>
#include "nodeinfomessage.h"
#include "abstracttask.h"
#include "regexhelper.h"
#include "transferstatusmessage.h"
#include <QProcess>

class TransferTask : public AbstractTask
{
    Q_OBJECT
public:
    void start();
    virtual void killTask();
    virtual void stopTask();
    TransferTask(int transferId, int transferType, QString localIp, QString remoteHost, int seconds, int remotePort, QObject *parent = 0);
signals:
    void newStatus(TransferStatusMessage &message);
private:
    QProcess process;
    QString remoteHost;
    QString localIp;
    RegexHelper regex;
    int seconds;
    int transferType;
    int remotePort;
private slots:
    void processFinished(int retvalue);
    void newOutput();
};

#endif // TRANSFERTASK_H
