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
    NodeInfoMessage *message = new NodeInfoMessage(this);
    message->data.listeningPort = "12345";
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        Interface interfaceStruct;
        interfaceStruct.interfaceName = interface.humanReadableName();
        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entries) {
            QHostAddress address = entry.ip();
            interfaceStruct.addresses << address.toString();
        }
        message->data.interfaces << interfaceStruct;
    }
    emit newInfoMessage(message);
    emit finished();
}
