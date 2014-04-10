#include "democore.h"
#include "sshconnection.h"
#include "connectedinfo.h"
#include <QThread>
#include "regexhelper.h"
DemoCore::DemoCore()
{

}

void DemoCore::connectToSlivers()
{
    QList<Sliver*> slivers = sliceManager.getSlivers();
    foreach (Sliver *sliver, slivers) {
        sliver->status = Sliver::STATUS_OFFLINE;
        sliverHash[sliver->name] = sliver;
        if (sliver->IPv6.isEmpty()) {
            qDebug() << "No ip, getting address";
            getIpAddress(sliver);
        } else {
            addSliverConnection(sliver);
        }
    }
}


void DemoCore::installProgram(Sliver *sliver)
{
    sliver->status = Sliver::STATUS_UPDATING;
    SSHConnection *ssh = new SSHConnection;
    ssh->addHost("henrik", "gatekeeper.nntb.no");
    ssh->addHost(sliver->sliceName, sliver->domain);
    ssh->executeCommand(QString("'sudo nohup wget --cache=off -N toki.dlinkddns.com/master/install.sh > nodeprog.out 2>&1; sudo nohup sh ./install.sh %1 > nodeprog.out 2>&1 &'").arg(VERSION));
    connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
    addSliverConnection(sliver);
    qDebug() << "Install sliver here2";
    connect(ssh, &SSHConnection::destroyed, []() {
        qDebug() << "Object deleted";
    });
}

void DemoCore::addSliverConnection(Sliver *sliver)
{
    qDebug() << "Connecting to sliver";
    addSliverConnection(sliver->IPv6, sliver->port, sliver->name);
}

void DemoCore::addSliverConnection(QString ip, int port, QString name)
{
    addConnection(ip, port, CONNECTION_TYPE_DEMO, name);
}

void DemoCore::start()
{
    connectToSlivers();
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
    ssh->addHost("henrik", "gatekeeper.nntb.no");
    ssh->addHost(sliver->sliceName, sliver->domain);
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

void DemoCore::pingHost(QString sliverName, QString host)
{
    Sliver *sliver = sliverHash[sliverName];
    DemoProtocol *protocol = protocolHash[sliverName];
    protocol->sendPingRequest(host);
}

Sliver* DemoCore::getSliver(QString name)
{
    Sliver *sliver = sliverHash[name];
    return sliver;
}

Sliver *DemoCore::getSliver(MyQTcpSocket *socket)
{
    return sliverHash[socket->getConnectionInfo().getName()];
}

AbstractProtocol *DemoCore::createProtocol(int type, MyQTcpSocket *socket)
{
    this->setParent(socket);
    if (type == CONNECTION_TYPE_DEMO) {
        DemoProtocol *protocol = new DemoProtocol(this);
        protocol->setSocket(socket);
        protocolHash[socket->getConnectionInfo().getName()] = protocol;
        connect(protocol, SIGNAL(newPingReply(PingReply,MyQTcpSocket*)), this, SLOT(handleNewPingReply(PingReply,MyQTcpSocket*)));
        connect(protocol, SIGNAL(nodeUpdating(MyQTcpSocket*)), this, SLOT(handleNodeUpdating(MyQTcpSocket*)));
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << type << "Should be " << CONNECTION_TYPE_DEMO;
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

void DemoCore::connectionError(QAbstractSocket::SocketError error)
{
    qDebug() << "COnnection problem";
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    Sliver *sliver= getSliver(socket);
    qDebug() << "Failed to connect: " << sliver->name;
    if (sliver->status == Sliver::STATUS_UPDATING) {
        qDebug()<< "Retrying connection";
        addSliverConnection(sliver); //retry connection
    } else if (sliver->status == Sliver::STATUS_INSTALLING) {
        addSliverConnection(sliver);
    } else {
        installProgram(sliver);
    }
    NetworkEntity::connectionError(error);
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
    emit newStatusMessage(*getSliver(socket), message);
}


