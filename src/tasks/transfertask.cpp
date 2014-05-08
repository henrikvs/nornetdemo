#include "transfertask.h"
#include "transferrequestmessage.h"


void TransferTask::start()
{
    if (transferType == TransferRequestMessage::TRANSFER_TYPE_TCP) {
        //process.start("sudo", QStringList() << "iperf" << "-c" << remoteHost << "-B" << localIp);
        process.start("iperf", QStringList() << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp);
        qDebug() << "Starting: " << QStringList() << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp;
    } else {
        qDebug() << "Undefined transfer type" << transferType;
    }
    connect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));
    connect(&process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
}

TransferTask::TransferTask(int transferId, int transferType, QString localIp, QString remoteHost, int seconds, QObject *parent):
    AbstractTask(transferId, parent), localIp(localIp), remoteHost(remoteHost), transferType(transferType), seconds(seconds)
{


}


void TransferTask::processFinished(int retvalue)
{
    if (!process.atEnd()) {
        process.close();
    }
    TransferStatusMessage message(taskId, 0, localIp, remoteHost, 0, TransferStatusMessage::STATE_FINISHED);
    emit newStatus(message);
    emit finished();
}

void TransferTask::newOutput()
{
    qDebug() << "New output";
    QString text = process.readAll();
    QString mbps = RegexHelper::getFirst(text, " (\\d*(?:\\.\\d+)?) Mbits\/sec");
    qDebug() << "mbps: " << mbps;
    TransferStatusMessage message(taskId, mbps, localIp, remoteHost, 0, TransferStatusMessage::STATE_RUNNING);
    emit newStatus(message);
}
