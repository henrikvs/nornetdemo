#ifndef TRANSFERTASK_H
#define TRANSFERTASK_H
#include <QObject>
#include "nodeinfomessage.h"
#include "nodeprotocol.h"
#include "abstracttask.h"
#include "regexhelper.h"
#include "transferstatusmessage.h"
#include <QProcess>

class TransferTask : public AbstractTask
{
    Q_OBJECT
public:
    void start();

    TransferTask(int transferId, int transferType, QString localIp, QString remoteHost, int seconds, QObject *parent = 0);
signals:
    void newStatus(TransferStatusMessage &message);
    void finished();
private:
    QProcess process;
    QString remoteHost;
    QString localIp;
    RegexHelper regex;
    int seconds;
    int transferType;
private slots:
    void processFinished(int retvalue);
    void newOutput();
};

#endif // TRANSFERTASK_H
