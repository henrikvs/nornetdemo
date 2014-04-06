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
    QList<Sliver> slivers = sliceManager.getSlivers();
    foreach (Sliver sliver, slivers) {
        sliverHash[sliver.IPv6] = sliver;
        addSliverConnection(sliver);
    }
}


void DemoCore::installProgram(Sliver sliver)
{
    SSHConnection *ssh = new SSHConnection;
    ssh->addHost("henrik", "gatekeeper.nntb.no");
    ssh->addHost(sliver.sliceName, sliver.domain);
    qDebug() << "Current thread: " << QThread::currentThread();
    sliver.IPv6 = "testetst";
    connect(ssh, &SSHConnection::outputSignal,[sliver, this](QString text) mutable {
       qDebug() << "Current thread: " << QThread::currentThread();
       qDebug() << "output: " + text;
       QString res = RegexHelper::getFirst(text, "ip:(.*)/64pi");
       if (!res.isEmpty()) {
           sliver.IPv6 = res;
           sliverHash[sliver.IPv6] = sliver;
           addSliverConnection(sliver); //We got the ip, now we try to reconnect. This is not thread safe. Needs to be invoked to be safe with different threads
       }
       qDebug() << "Result: " << res;
    });
    ssh->executeCommand("ip addr show eth0 | grep 'inet6 2001:700' | awk 'NR==1 {print \"ip:\"$2\"pi\"}'");
    ssh->executeCommand(QString("'sudo nohup wget --cache=off -N toki.dlinkddns.com/master/install.sh > nodeprog.out 2>&1; sudo nohup sh ./install.sh %1 > nodeprog.out 2>&1 &'").arg(VERSION));
    qDebug() << "Install sliver here2";
}

void DemoCore::addSliverConnection(Sliver sliver)
{
    qDebug() << "Connecting to sliver";
    addSliverConnection(sliver.IPv6, sliver.port);
}

void DemoCore::addSliverConnection(QString ip, int port)
{
    addConnection(ip, port, CONNECTION_TYPE_DEMO);
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

Sliver DemoCore::getSliver(int socketId)
{
    MyQTcpSocket *socket = getSocket(socketId);
    QString address = socket->getConnectionInfo().getAddress();
    Sliver sliver = sliverHash[address];
    return sliver;
}

void DemoCore::setSliver(Sliver sliver, int id)
{
    MyQTcpSocket *socket = getSocket(id);
    QString address = socket->getConnectionInfo().getAddress();
    sliverHash[address] = sliver;
}



AbstractProtocol *DemoCore::createProtocol(int type, MyQTcpSocket *socket)
{
    if (type == CONNECTION_TYPE_DEMO) {
        DemoProtocol *protocol = new DemoProtocol(getNextId(), this);
        protocol->setSocket(socket);
        connect(protocol, SIGNAL(newPingReply(PingReply,int)), this, SLOT(pingReplySlot(PingReply,int)));
        connect(protocol, SIGNAL(nodeUpdating(int)), this, SLOT(handleNodeUpdating(int)));
        protocolHash[protocol->getId()] = protocol;
        return protocol;
    } else {
        qDebug() << "Unrecognized protocol" << type << "Should be " << CONNECTION_TYPE_DEMO;
        return NULL;
    }
}

void DemoCore::sliverConnected()
{
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    ConnectionInfo info = socket->getConnectionInfo();
    Sliver sliver = sliverHash[info.getAddress()];
    qDebug() << "Connected: " << sliver.IPv6;
}

void DemoCore::connectionError(QAbstractSocket::SocketError error)
{
    qDebug() << "COnnection problem";
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    ConnectionInfo info = socket->getConnectionInfo();
    if (sliverHash.contains(info.getAddress())) {
        Sliver sliver= sliverHash[info.getAddress()];
        qDebug() << "Failed to connect: " << sliver.IPv6;
        if (sliver.status == Sliver::STATUS_UPDATING) {
            qDebug()<< "Retrying connection";
            addSliverConnection(sliver); //retry connection
        } else {
            sliver.status = Sliver::STATUS_UPDATING;
            sliverHash[info.getAddress()] = sliver;
            installProgram(sliver);
        }
    }
    removeSocket(socket->getId());
}

bool DemoCore::newStdIn(QString input)
{
    //if (NetworkEntity::newStdIn(input)) {
        //return true;
    //}

    QStringList tokens = input.split(QRegExp("\\s"));

    if (tokens[0] == "connect") {
            addSliverConnection(tokens[1], tokens[2].toInt());
            qDebug() << "Connecting to: " << tokens[1] << ":" << tokens[2];
    }
    return true;
}

void DemoCore::pingReplySlot(PingReply message, int id)
{
    emit newPingReply(getSliver(id), message);
}

void DemoCore::nodeInfoSlot(NodeInfoMessage message, int id)
{
    emit newStatusMessage(getSliver(id), message);
}

void DemoCore::handleNodeUpdating(int nodeId)
{
    Sliver sliver = getSliver(nodeId);
    sliver.status = Sliver::STATUS_UPDATING;
    setSliver(sliver, nodeId);
}


