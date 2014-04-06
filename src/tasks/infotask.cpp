#include "infotask.h"
#include "regexhelper.h"
#include "nodeinfomessage.h"
#include <QNetworkInterface>
#include <QList>
#include <QNetworkAddressEntry>
#include <QHostAddress>


InfoTask::InfoTask(QObject *parent) :
    QObject(parent)
{
    //regex.addRegex("time=(\\d+(.\\d*)?)\\s*ms", "ms");
}

void InfoTask::start() {

    message.data.listeningPort = "12345";
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString address;
    foreach (QNetworkInterface interface, interfaces) {
        Interface interfaceStruct;
        interfaceStruct.interfaceName = interface.humanReadableName();
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        if (address.isEmpty() && interface.humanReadableName() == "eth0") {
            address = entries[0].ip().toString(); //we will use this ip for reverse lookup
        }
        foreach (QNetworkAddressEntry entry, entries) {
            QHostAddress address = entry.ip();
            interfaceStruct.addresses << address.toString();
        }
        message.data.interfaces << interfaceStruct;
    }



    if (!address.isEmpty()) {
        doReverseLookup(address);
    } else {
        qDebug() << "address not found";
        emit newInfoMessage(message);
        emit finished();
    }
}



void InfoTask::newDnsOutput()
{
    dnsResult += dnsProcess.readAll();
}

void InfoTask::newReverseDnsOutput()
{
    revDnsResult += revDnsProcess.readAll();
}

void InfoTask::revDnsFinished(int)
{
    if (!revDnsProcess.atEnd()) {
        revDnsProcess.close();
    }

    QString result = RegexHelper::getFirst(revDnsResult, "PTR\\s*(\\S*)\\.");
    qDebug() << "Host string: " << revDnsResult;
    if (!result.isEmpty()) {
        qDebug() << "hostname found: " << result;
        dnsProcess.start("dig", QStringList() << result <<"loc"<<"+noall" <<"+answer");
        connect(&dnsProcess, SIGNAL(readyRead()), this, SLOT(newDnsOutput()));
        connect(&dnsProcess, SIGNAL(finished(int)), this, SLOT(dnsFinished(int)));
    } else {
        qDebug()<< "Failed to do reverse lookup";
        emit newInfoMessage(message);
        emit finished();
    }
}

void InfoTask::dnsFinished(int retvalue)
{
    if (!dnsProcess.atEnd()) {
        dnsProcess.close();
    }

    QStringList result = RegexHelper::getFirstGroup(dnsResult, "LOC\\s*(\\d.*)\\s+N\\s*(\\d.*)\\s+E");
    qDebug() << "Location string: " << dnsResult;
    if (result.size() == 3) {
        qDebug() << "location found: " << result[1] << result[2];
        message.data.lat = result[1]; //todo
        message.data.lng = result[2]; //todo

    } else {
        qDebug()<< "Failed to find location";
    }
    emit newInfoMessage(message);
    emit finished();
}

void InfoTask::doReverseLookup(QString address)
{
    qDebug() << "Doing reverse lookup on: " << address;
    connect(&revDnsProcess, SIGNAL(readyRead()), this, SLOT(newReverseDnsOutput()));
    connect(&revDnsProcess, SIGNAL(finished(int)), this, SLOT(revDnsFinished(int)));
    revDnsProcess.start("dig", QStringList() << "-x" << address << "+noall"<< "+answer");
}
