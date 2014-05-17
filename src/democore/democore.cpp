#include "democore.h"
#include "sshconnection.h"
#include "connectedinfo.h"
#include <QThread>
#include <QString>
#include <QTimer>
#include "regexhelper.h"
DemoCore::DemoCore(): NetworkEntity(), gateKeeperEnabled(false)
{
}

void DemoCore::connectToSlivers(QList<Sliver*> slivers)
{
    foreach (Sliver *sliver, slivers) {
        sliver->status = Sliver::STATUS_OFFLINE;
        sliverHash[sliver->name] = sliver;
        if (relayEnabled()) {
            installProgram(sliver);
            addSliverConnection(sliver);
        } else {
            if (sliver->IPv6.isEmpty()) {
                qDebug() << "No ip, getting address";
                getIpAddress(sliver);
                installProgram(sliver);
            } else {
                addSliverConnection(sliver);
                QTimer *timer = new QTimer(this);
                //if there's a specified IP address, give the connection some time before running the script setting things up.
                connect(timer, &QTimer::timeout, [timer, sliver, this]() {
                    qDebug() << "Checking connection status";
                    if (sliver->status == sliver->STATUS_OFFLINE) {
                        qDebug() << "Still offline, reinstalling";
                        installProgram(sliver);
                    }
                    timer->stop();
                    timer->deleteLater();
                });
                timer->start(3000);
            }
        }

        QTimer *timer = new QTimer(this);


        //Will try to connect as long as the sliver is not connected
        connect(timer, &QTimer::timeout, [timer, sliver, this]() {
            if (sliver->status != sliver->STATUS_CONNECTED) {
                qDebug() << "Retryign connection";
                addSliverConnection(sliver);
            } else {
                timer->stop();
                timer->deleteLater();
            }

            static int time = 0;
            time+=3;
            //if (time > 35) {
             //   shutDownNodeprogs(QList<Sliver*>() << sliver);
              //  installProgram(sliver);
               // time = 0;
           // }
        });
        timer->start(3000);
    }

}

void DemoCore::shutDownNodeprogs(QList<Sliver *> slivers)
{
    foreach (Sliver *sliver, slivers) {
        SSHConnection *ssh = new SSHConnection;
        if (gateKeeperEnabled) {
            ssh->addHost(gatekeeperUsername, gatekeeperHostname);
        }
        ssh->addHost(sliver->sliceName, sliver->hostName);

        ssh->executeCommand(QString("'sudo kill -INT nodeprog'"));
        connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
        qDebug() << "Killing nodeprog" << sliver->name;
        connect(ssh, &SSHConnection::destroyed, []() {
            qDebug() << "Object deleted";
        });
    }

}

void DemoCore::disconnected(MyQTcpSocket *socket)
{
    qDebug() << "Disconnecting nodeprog";
    /*if (socket->isRelay) {
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, getName());
    }*/

    qDebug() << "COnnection problem";
    Sliver *sliver= getSliver(socket);
    protocolHash.remove(sliver->name);
    qDebug() << "Failed to connect: " << sliver->name;
    emit sliverDisonnected(*sliver);
    if (sliver->status == Sliver::STATUS_UPDATING) {
        emit sliverUpdating(*sliver);
        qDebug()<< "Retrying connection";
        //addSliverConnection(sliver); //retry connection
    } else if (sliver->status == Sliver::STATUS_INSTALLING) {
        //addSliverConnection(sliver);
    } else {
        //installProgram(sliver);
        sliver->status = Sliver::STATUS_OFFLINE;
        //addSliverConnection(sliver);
    }
}

QStringList DemoCore::getIpv4List(QString name)
{
    QStringList addresses;
    Sliver *sliver = sliverHash[name];
    foreach(NetworkInterface interface, sliver->infoMessage.data.interfaces) {
        //if (interface.interfaceName == "eth0") {
        qDebug() << "Address";
            foreach(QString address, interface.addresses) {
                if (address.contains(".")) {
                    addresses<< address;
                }
            }
        //}
    }
    return addresses;
}

QStringList DemoCore::getIpv6List(QString name)
{
    QStringList addresses;
    Sliver *sliver = sliverHash[name];
    foreach(NetworkInterface interface, sliver->infoMessage.data.interfaces) {
        //if (interface.interfaceName == "eth0") {
         qDebug() << "Address";
            foreach(QString address, interface.addresses) {
                if (address.contains(":")) {
                    addresses<< address;
                }
            }
        //}
    }
    return addresses;
}




void DemoCore::installProgram(Sliver *sliver)
{
    sliver->status = Sliver::STATUS_INSTALLING;
    SSHConnection *ssh = new SSHConnection;
    if (gateKeeperEnabled) {
        ssh->addHost(gatekeeperUsername, gatekeeperHostname);
    }
    ssh->addHost(sliver->sliceName, sliver->hostName);
    QString relayString = "";
    if (relayEnabled()) {
        relayString = getRelayAddress() + ":" + QString::number(getRelayPort());
        qDebug() << "Starting with relaystring:" << relayString;
    }

    QString checkExistsCommand = "ps aux | grep install.sh | grep -v grep | wc -l";

    ssh->executeCommand(QString("'sudo nohup wget --cache=off -N %3/install.sh >> nodeprog.out 2>&1; sudo nohup sh ./install.sh %1 %3 %2>> nodeprog.out 2>&1 &'").arg(VERSION).arg(relayString).arg(nodeprogRootUrl));
    qDebug() << "install script: " << QString("'sudo nohup wget --cache=off -N %3/install.sh >> nodeprog.out 2>&1; sudo nohup sh ./install.sh %1 %2>> nodeprog.out 2>&1 &'").arg(VERSION).arg(relayString).arg(nodeprogRootUrl);
    connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
    //addSliverConnection(sliver);
    qDebug() << "Install sliver here2";
    connect(ssh, &SSHConnection::destroyed, []() {
        qDebug() << "Object deleted";
    });
}

void DemoCore::setnodeprogRootUrl(QString url)
{
    this->nodeprogRootUrl = url;
}

void DemoCore::addSliverConnection(Sliver *sliver)
{
    qDebug() << "Connecting to sliver" << sliver->IPv6 << sliver->port << sliver->name;
    addSliverConnection(sliver->IPv6, sliver->port, sliver->sliceName, sliver->hostName);
}

void DemoCore::addSliverConnection(QString ip, int port, QString sliceName, QString hostName)
{
    if (relayEnabled()) {
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, sliceName, hostName);
    } else {
        addConnection(ip, port, CONNECTION_TYPE_DEMO, sliceName, hostName);
    }
}

void DemoCore::start()
{
    //connectToSlivers();
}

void DemoCore::shutDownNodes()
{
    foreach (DemoProtocol *protocol, protocolHash) {
        protocol->sendExit();
    }
}

void DemoCore::getIpAddress(Sliver *sliver)
{
    SSHConnection *ssh = new SSHConnection;
    if (gateKeeperEnabled) {
        ssh->addHost(gatekeeperUsername, gatekeeperHostname);
    }
    ssh->addHost(sliver->sliceName, sliver->hostName);
    connect(ssh, &SSHConnection::outputSignal,[sliver, this](QString text) mutable {
       qDebug() << "Current thread: " << QThread::currentThread();
       qDebug() << "output: " + text;
       QString res = RegexHelper::getFirst(text, "ip:(.*)/64pi");
       if (!res.isEmpty()) {
           sliver->IPv6 = res;
           qDebug() << "Ip address set";
           addSliverConnection(sliver); //We got the ip, now we try to reconnect. This is not thread safe. Needs to be invoked to be safe with different threads
       }
       qDebug() << "Result: " << res;
    });
    connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
    connect(ssh, &SSHConnection::destroyed, []() {
        qDebug() << "Object deleted";
    });


    ssh->executeCommand("ip addr show eth0 | grep 'inet6 2001:700' | awk 'NR==1 {print \"ip:\"$2\"pi\"}'");
}

int DemoCore::pingHost(QString sliverName, QString host, QString localIp, int seconds)
{
    if (!protocolHash.contains(sliverName)) {
        qDebug() << "Unknown protocol";
        return -1;
    }
    DemoProtocol *protocol = protocolHash[sliverName];
    int id = nextRequestId();
    protocol->sendPingRequest(id, host, localIp, seconds);
    return id;
}

int DemoCore::transferRequest(QString sliverName, QString host, QString localIp, int transferType, int seconds)
{
    if (!protocolHash.contains(sliverName)) {
        qDebug() << "Unknown protocol";
        return -1;
    }

    DemoProtocol *protocol = protocolHash[sliverName];
    int id = nextRequestId();
    protocol->sendTransferRequest(id, host, localIp, transferType, seconds);
    return id;
}

void DemoCore::stopExperiment(QString sliverName, int sessionId)
{
    if (!protocolHash.contains(sliverName)) {
        qDebug() << "Unknown protocol";
        return;
    }

    DemoProtocol *protocol = protocolHash[sliverName];
    protocol->sendStopTask(sessionId);
}

void DemoCore::enableGatekeeper(QString username, QString hostname)
{
    qDebug() << "Gatekeeper enabled" << username << hostname;
    this->gatekeeperUsername = username;
    this->gatekeeperHostname = hostname;
    gateKeeperEnabled = true;
}

void DemoCore::disableGatekeeper()
{
    gateKeeperEnabled = false;
}

Sliver* DemoCore::getSliver(QString name)
{
    Sliver *sliver = sliverHash[name];
    return sliver;
}

Sliver *DemoCore::getSliver(MyQTcpSocket *socket)
{
    return sliverHash[socket->getConnectionInfo().getHostName()];
}

AbstractProtocol *DemoCore::createProtocol(HandshakeMessage message, MyQTcpSocket *socket)
{
    this->setParent(socket);
    if (message.data.connectionType == CONNECTION_TYPE_DEMO || message.data.connectionType == CONNECTION_TYPE_RELAY || message.data.connectionType == CONNECTION_TYPE_REPLY) {
        DemoProtocol *protocol = new DemoProtocol(this);
        protocol->setSocket(socket);
        protocolHash[socket->getConnectionInfo().getHostName()] = protocol;
        connect(protocol, SIGNAL(newPingReply(PingReply,MyQTcpSocket*)), this, SLOT(handleNewPingReply(PingReply,MyQTcpSocket*)));
        connect(protocol, SIGNAL(nodeUpdating(MyQTcpSocket*)), this, SLOT(handleNodeUpdating(MyQTcpSocket*)));
        connect(protocol, SIGNAL(newNodeInfo(NodeInfoMessage,MyQTcpSocket*)), this, SLOT(handleNewNodeInfo(NodeInfoMessage,MyQTcpSocket*)));
        connect(protocol, SIGNAL(newTransferStatus(TransferStatusMessage,MyQTcpSocket*)), this, SLOT(handleNewTransferStatus(TransferStatusMessage,MyQTcpSocket*)));
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << message.data.connectionType << "Should be " << CONNECTION_TYPE_DEMO;
        return NULL;
    }
}

void DemoCore::connected()
{
    NetworkEntity::connected();
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    Sliver *sliver = getSliver(socket);
    sliver->status = Sliver::STATUS_CONNECTED;
    qDebug() << "Connected: " << sliver->name;
}

void DemoCore::handleSSHDisconnect()
{
    qDebug() << "ssh disconnected";
}

bool DemoCore::newStdIn(QString input)
{
    //if (NetworkEntity::newStdIn(input)) {
        //return true;
    //}

    QStringList tokens = input.split(QRegExp("\\s"));

    /*if (tokens[0] == "connect") {
            addSliverConnection(tokens[1], tokens[2].toInt());
            qDebug() << "Connecting to: " << tokens[1] << ":" << tokens[2];
    }*/
    return true;
}

void DemoCore::handleNodeUpdating(MyQTcpSocket *socket)
{
    Sliver *sliver = getSliver(socket);
    sliver->status = Sliver::STATUS_UPDATING;
}

void DemoCore::handleNewPingReply(PingReply message, MyQTcpSocket *socket)
{
    emit newPingReply(*getSliver(socket), message);
}

void DemoCore::handleNewNodeInfo(NodeInfoMessage message, MyQTcpSocket *socket)
{
    /*
    if (!message.data.name.isEmpty() && relayEnabled()) { //hack
        qDebug() << "Updating slice";
        QString name = message.data.name;
        if (name != getSliver(socket)->name) {
            qDebug() << "Doing swapping";
            ConnectionInfo info("", 0, 0, name);
            socket->setConnectionInfo(info);
            DemoProtocol *tmpProt = protocolHash[name];
            protocolHash[name] = (DemoProtocol*)socket->getProtocol();
            protocolHash[getSliver(socket)->name] = tmpProt;
        }
    }*/
    getSliver(socket)->infoMessage = message;

    emit newStatusMessage(*getSliver(socket), message);
}

void DemoCore::handleNewTransferStatus(TransferStatusMessage message, MyQTcpSocket *socket)
{
    emit newTransferStatus(*getSliver(socket), message);
}

int DemoCore::getEntityType()
{
    return NetworkEntity::ENTITY_TYPE_DEMO;
}

