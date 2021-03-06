#ifndef INFOTASK_H
#define INFOTASK_H
#include <QObject>
#include "nodeinfomessage.h"
#include "abstracttask.h"
#include <QProcess>
/*!
 * \brief Used to collect information about the machine it's running on.
 * The result is extracted by connecting to the ::newInfoMessage signal.
 */
class InfoTask : public AbstractTask
{
    Q_OBJECT
public:
    explicit InfoTask(QObject *parent = 0);
    virtual void killTask();
    virtual void stopTask();
public slots:
    void start();
private slots:
    void newDnsOutput();
    void newReverseDnsOutput();

    void revDnsFinished(int retvalue);
    void dnsFinished(int retvalue);
signals:
    void newInfoMessage(NodeInfoMessage &message);
private:
    QProcess revDnsProcess;
    QProcess dnsProcess;
    QString dnsResult;
    QString revDnsResult;
    void doReverseLookup(QString address);
    NodeInfoMessage message;
};

#endif // STATUSTASK_H
