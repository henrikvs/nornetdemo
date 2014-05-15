#include "transfertask.h"
#include "transferrequestmessage.h"


void TransferTask::start()
{
    bool ipv4 = false;
    if (localIp.contains(".")) {
        ipv4 = true;
    } else {
        ipv4 = false;
    }

    if (transferType == TransferRequestMessage::TRANSFER_TYPE_TCP) {
        //process.start("sudo", QStringList() << "iperf" << "-c" << remoteHost << "-B" << localIp);
        if (ipv4) {
            process.start("iperf", QStringList() << "-p" << QString::number(remotePort) << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp);
            qDebug() << "Starting iperf ipv4: " << QStringList() << "-p" << QString::number(remotePort) << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp;
        } else { //IPv6
            process.start("iperf", QStringList() << "-p" << QString::number(remotePort) << "-V" << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp);
            qDebug() << "Starting iperf ipv6: " << QStringList() << "-p" << QString::number(remotePort) << "-V" << "-t" << QString::number(seconds) << "-i" << "1" << "-f" << "m" << "-c" << remoteHost << "-B" << localIp;
        }
    } else {
        qDebug() << "Undefined transfer type" << transferType;
    }
    connect(&process, SIGNAL(readyRead()), this, SLOT(newOutput()));
    connect(&process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
}

void TransferTask::killTask()
{
    process.kill();
}

void TransferTask::stopTask()
{
    process.terminate();
}

TransferTask::TransferTask(int transferId, int transferType, QString localIp, QString remoteHost,  int seconds,  int remotePort, QObject *parent):
    AbstractTask(transferId, parent), localIp(localIp), remoteHost(remoteHost), transferType(transferType), seconds(seconds), remotePort(remotePort)
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
