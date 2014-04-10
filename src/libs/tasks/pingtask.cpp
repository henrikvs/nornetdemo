#include "pingtask.h"
#include "abstractmessage.h"

PingTask::PingTask(QString remoteHost, QObject *parent) :
    AbstractTask(parent), remoteHost(remoteHost)
{
    regex.addRegex("time=(\\d+(.\\d*)?)\\s*ms", "ms");
}

PingTask::~PingTask()
{
    disconnect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));
    qDebug() << "Closing pingtask";
    if (process.isOpen()) {
        process.terminate();
        process.waitForFinished();
        qDebug() << "Closed process";
    }
}

void PingTask::start()
{
    process.start("sudo", QStringList() << "ping" << remoteHost);
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
    PingReply reply(values[1]);
    emit newPing(reply);
}
