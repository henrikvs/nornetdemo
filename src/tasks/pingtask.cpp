#include "pingtask.h"
#include "abstractmessage.h"

PingTask::PingTask(QString remoteHost, QObject *parent) :
    QObject(parent), remoteHost(remoteHost)
{
    regex.addRegex("time=(\\d+(.\\d*)?)\\s*ms", "ms");
}

void PingTask::start()
{
    process.start("ping", QStringList() << remoteHost);
    connect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));
    connect(&process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    connect(this, SIGNAL(newString(QString)), &regex, SLOT(slotNewInput(QString)));
    connect(&regex, SIGNAL(signalNewMatch(QStringList,QString)), this, SLOT(newMatch(QStringList,QString)));
}

void PingTask::processFinished(int retvalue)
{
    if (!process.atEnd()) {
        process.close();
    }
    emit finished();
}

void PingTask::newOutput()
{
    QString output = process.readAll();
    emit newString(output);
}

void PingTask::newMatch(QStringList values, QString id)
{
    PingReply *reply = new PingReply(values[1], this);
    emit newPing(reply);
}
