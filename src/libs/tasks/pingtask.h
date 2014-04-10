#ifndef PINGTASK_H
#define PINGTASK_H

#include <QObject>
#include <QProcess>
#include "pingreply.h"
#include "regexhelper.h"
#include "abstracttask.h"

class PingTask : public AbstractTask
{
    Q_OBJECT
public:
    explicit PingTask(QString remoteHost, QObject *parent = 0);
    virtual ~PingTask();
    int taskNumber;
signals:
    void newPing(PingReply &message);
    void newString(QString string);
    void finished();
public slots:
    void start();
private slots:
    void processFinished(int retvalue);
    void newOutput();
    void newMatch(QStringList values, QString id);
private:
    QProcess process;
    QString remoteHost;
    RegexHelper regex;

};

#endif // PINGTASK_H
