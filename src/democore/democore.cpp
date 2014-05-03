#include "democore.h"
#include "sshconnection.h"
#include "connectedinfo.h"
#include <QThread>
#include <QString>
#include "regexhelper.h"
DemoCore::DemoCore(): gateKeeperEnabled(false)
{
}

void DemoCore::connectToSlivers(QList<Sliver*> slivers)
{
    foreach (Sliver *sliver, slivers) {
        sliver->status = Sliver::STATUS_OFFLINE;
        sliverHash[sliver->name] = sliver;
        if (sliver->IPv6.isEmpty()) {
            qDebug() << "No ip, getting address";
            getIpAddress(sliver);
            //installProgram(sliver);
        } else {
            addSliverConnection(sliver);
        }
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
    qDebug() << "Failed to connect: " << sliver->name;
    if (sliver->status == Sliver::STATUS_UPDATING) {
        qDebug()<< "Retrying connection";
        addSliverConnection(sliver); //retry connection
    } else if (sliver->status == Sliver::STATUS_INSTALLING) {
        addSliverConnection(sliver);
    } else {
        installProgram(sliver);
        addSliverConnection(sliver);
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
    ssh->executeCommand(QString("'sudo nohup wget --cache=off -N toki.dlinkddns.com/master/install.sh > nodeprog.out 2>&1; sudo nohup sh ./install.sh %1 %2> nodeprog.out 2>&1 &'").arg(VERSION).arg(sliver->name));
    connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
    //addSliverConnection(sliver);
    qDebug() << "Install sliver here2";
    connect(ssh, &SSHConnection::destroyed, []() {
        qDebug() << "Object deleted";
    });
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

void DemoCore::pingHost(QString sliverName, QString host, QString localIp)
{
    Sliver *sliver = sliverHash[sliverName];
    DemoProtocol *protocol = protocolHash[sliverName];
    protocol->sendPingRequest(nextRequestId(),host, localIp);
}

int DemoCore::transferRequest(QString sliverName, QString host, QString localIp, int transferType, int seconds)
{
    Sliver *sliver = sliverHash[sliverName];
    DemoProtocol *protocol = protocolHash[sliverName];
    int id = nextRequestId();
    protocol->sendTransferRequest(id, host, localIp, seconds, transferType);
    return id;
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

