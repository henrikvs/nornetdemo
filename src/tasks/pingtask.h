#ifndef PINGTASK_H
#define PINGTASK_H

#include <QObject>
#include <QProcess>
#include "pingreply.h"
#include "regexhelper.h"

class PingTask : public QObject
{
    Q_OBJECT
public:
    explicit PingTask(QString remoteHost, QObject *parent = 0);
    int taskNumber;
signals:
    void newPing(PingReply *message);
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
