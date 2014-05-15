#include "pingtask.h"
#include "abstractmessage.h"

PingTask::PingTask(int taskId, QString remoteHost, QString localIp, int time, QObject *parent) :
    AbstractTask(taskId, parent), remoteHost(remoteHost), localIp(localIp), time(time)
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

void PingTask::killTask()
{
    process.kill();
}

void PingTask::stopTask()
{
    process.terminate();
}

void PingTask::start()
{
    QString program;
    if (localIp.contains(":")) {
        qDebug() << "ping6";
        program = "ping6";
    } else {
        qDebug() << "ping";
        program = "ping";
    }
    if (this->time == 0) {
        process.start("sudo", QStringList() << program << "-I" << localIp << remoteHost);
    } else {
        process.start("sudo", QStringList() << program << "-I" << localIp <<"-w" << QString::number(time) << remoteHost);
    }
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
    PingReply reply(0, localIp, remoteHost, taskId, PingReply::STATE_FINISHED);
    emit newPing(reply);
    emit finished();
}

void PingTask::newOutput()
{
    QString output = process.readAll();
    emit newString(output);
}

void PingTask::newMatch(QStringList values, QString id)
{
    PingReply reply(values[1], localIp, remoteHost, taskId, PingReply::STATE_RUNNING);
    emit newPing(reply);
}
