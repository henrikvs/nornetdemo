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

/**
 * @brief Attempts to connect to the given nodes.
 * If there's no IP address associated with a node, an ssh connection will first be
 * established to fetch a globally available IPv6 address, and also deploy the connection
 * program on the node. It will register timers, to keep trying to connect regularly.
 *
 * @param slivers A list of slivers, including at least the hostname of the node.
 */
void DemoCore::connectToSlivers(QList<Sliver*> slivers)
{
    foreach (Sliver *sliver, slivers) {
        setStatus(sliver, Sliver::STATUS_OFFLINE);
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
                    if (sliver->getStatus() == sliver->STATUS_OFFLINE) {
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
            if (sliver->getStatus() != sliver->STATUS_CONNECTED) {
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

/**
 * @brief Establishes SSH connection with the given slivers, and forcefully shuts
 * down any running nodeprog instances on these. It is intended to be used only if normal shut down is
 * not possible.
 * @param slivers Slivers to connect to.
 */
void DemoCore::shutDownNodesSSH(QList<Sliver *> slivers)
{
    foreach (Sliver *sliver, slivers) {
        SSHConnection *ssh = new SSHConnection;
        if (gateKeeperEnabled) {
            ssh->addHost(gatekeeperUsername, gatekeeperHostname);
        }
        ssh->addHost(getSliceName(), sliver->hostName);

        ssh->executeCommand(QString("'sudo kill -INT nodeprog'"));
        connect(ssh, SIGNAL(disconnected()), ssh, SLOT(deleteLater()));
        qDebug() << "Killing nodeprog" << sliver->name;
        connect(ssh, &SSHConnection::destroyed, []() {
            qDebug() << "Object deleted";
        });
    }

}

void DemoCore::disconnectNodes(QList<Sliver *> slivers)
{
    foreach (Sliver *sliver, slivers) {
        if (protocolHash.contains(sliver->hostName)) {
            DemoProtocol *protocol  = protocolHash.take(sliver->hostName);
            protocol->disconnectSocket();
        }
    }
}
/**
 * @brief Handles a disconnected socket. Will do cleanup and emit a signal that may
 * be handles by the GUI to do it's clean-up
 * @param socket socket that is disconnected
 */
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
    if (sliver->getStatus() == Sliver::STATUS_CONNECTED) { //if the node was connected, we give a notice that it has now disconnected
        emit sliverDisonnected(*sliver);
    }
    if (sliver->getStatus() == Sliver::STATUS_UPDATING) {
        emit sliverUpdating(*sliver);
        qDebug()<< "Retrying connection";
        //addSliverConnection(sliver); //retry connection
    } else if (sliver->getStatus() == Sliver::STATUS_INSTALLING) {
        //addSliverConnection(sliver);
    } else {
        //installProgram(sliver);
        setStatus(sliver, Sliver::STATUS_OFFLINE);
        //addSliverConnection(sliver);
    }
}

/**
 * @brief Returns a list of all IPv4 addresses for a node.
 * @param name The name of the node
 * @return A list of all IPv4 addresses
 */
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

/**
 * @brief  Returns a list of all IPv6 addresses for a node.
 * @param name The name of the node
 * @return A list of all IPv6 addresses.
 */
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

QString DemoCore::getSliceName()
{
   return sliceName;
}

void DemoCore::setActiveSlice(QString sliceName)
{
    this->sliceName = sliceName;
}



/**
 * @brief Deploys nodeprog and it's dependencies on a node.
 * @param sliver The sliver to depoy on.
 */
void DemoCore::installProgram(Sliver *sliver)
{
    setStatus(sliver, Sliver::STATUS_INSTALLING);
    SSHConnection *ssh = new SSHConnection;
    if (gateKeeperEnabled) {
        ssh->addHost(gatekeeperUsername, gatekeeperHostname);
    }
    ssh->addHost(getSliceName(), sliver->hostName);
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

/**
 * @brief Sets the url of the install script and nodeprog program.
 * @param url Should point to the directory where the install script and nodeprog
 * is located, excluding trailing /
 */
void DemoCore::setnodeprogRootUrl(QString url)
{
    this->nodeprogRootUrl = url;
}

/**
 * @brief Connects to a sliver. The sliver should be running nodeprog.
 * @param sliver Sliver to connect to.
 */
void DemoCore::addSliverConnection(Sliver *sliver)
{
    if (sliver->getStatus() != Sliver::STATUS_CONNECTED && sliver->getStatus() != Sliver::STATUS_CONNECTING) {
        setStatus(sliver, Sliver::STATUS_CONNECTING);
        qDebug() << "Connecting to sliver" << sliver->IPv6 << sliver->port << sliver->name;
        addSliverConnection(sliver->IPv6, sliver->port, getSliceName(), sliver->hostName);
    }
}

/**
 * @brief Connects to a sliver. The sliver should be running nodeprog.
 * @param ip
 * @param port
 * @param sliceName
 * @param hostName
 */
void DemoCore::addSliverConnection(QString ip, int port, QString sliceName, QString hostName)
{
    if (relayEnabled()) {
        addConnection(getRelayAddress(), getRelayPort(), CONNECTION_TYPE_RELAY, sliceName, hostName);
    } else {
        addConnection(ip, port, CONNECTION_TYPE_DEMO, sliceName, hostName);
    }
}

/**
 * @brief Initializations upon starting
 */
void DemoCore::start()
{
    //connectToSlivers();
}

/**
 * @brief Send a shut down command to the connected nodes, instructing them to disconnect and shut down themselves.
 */
void DemoCore::shutDownNodes()
{
    foreach (DemoProtocol *protocol, protocolHash) {
        protocol->sendExit();
    }
}

/**
 * @brief Uses SSH to get the IPv6 address of a sliver. The address is asynchronously saved in the given sliver
 * object when the address is found.
 * @param sliver
 */
void DemoCore::getIpAddress(Sliver *sliver)
{
    SSHConnection *ssh = new SSHConnection;
    if (gateKeeperEnabled) {
        ssh->addHost(gatekeeperUsername, gatekeeperHostname);
    }
    ssh->addHost(getSliceName(), sliver->hostName);
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

/**
 * @brief Sends a request to start a ping session with the given node
 * @param sliverName Name of the node
 * @param host Remote address to ping. Should be an IPv6 or IPv4 address.
 * @param localIp Local ip to bind to.
 * @param seconds Duration of the ping experiment.
 * @return The unique id of this session. Used to identify this session later.
 */
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

/**
 * @brief Sends a request to start a new transfer session with the given node
 * @param sliverName Name of the node.
 * @param host IP Address to start the transfer session with. Can be IPv6 or Ipv4, but no host name.
 * @param localIp Local ip to bind to.
 * @param transferType What kind of transfer to make. Currently, only TransferRequestMessage::TRANSFER_TYPE_TCP
 * is supported.
 * @param seconds Duration of the transfer test in seconds.
 * @return The unique session id that is used to identify this session.
 */
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

/**
 * @brief Used to stop a running experiment.(Ping or TCP)
 * @param sliverName Name of the sliver.
 * @param sessionId Unique ID of the session to close.
 * Session ID is the id returned when the experiment is first started.
 *
 */
void DemoCore::stopExperiment(QString sliverName, int sessionId)
{
    if (!protocolHash.contains(sliverName)) {
        qDebug() << "Unknown protocol";
        return;
    }

    DemoProtocol *protocol = protocolHash[sliverName];
    protocol->sendStopTask(sessionId);
}
/**
 * @brief Enables the gatekeeper.
 * @param username Username to connect with.
 * @param hostname The hostname of the gatekeeper
 */
void DemoCore::enableGatekeeper(QString username, QString hostname)
{
    qDebug() << "Gatekeeper enabled" << username << hostname;
    this->gatekeeperUsername = username;
    this->gatekeeperHostname = hostname;
    gateKeeperEnabled = true;
}

/**
 * @brief Disables the gatekeeper
 */
void DemoCore::disableGatekeeper()
{
    gateKeeperEnabled = false;
}

/**
 * @brief Get the sliver object from the sliver name
 * @param name Name of the node
 * @return Sliver object
 */
Sliver* DemoCore::getSliver(QString name)
{
    Sliver *sliver = sliverHash[name];
    return sliver;
}

/**
 * @brief Get the sliver associated with the socket
 * @param socket
 * @return
 */
Sliver *DemoCore::getSliver(MyQTcpSocket *socket)
{
    return sliverHash[socket->getConnectionInfo().getHostName()];
}

/**
 * @brief Create a protocol based on the handshake
 * @param message The handhake message
 * @param socket connected socket
 * @return
 */
AbstractProtocol *DemoCore::createProtocol(HandshakeMessage message, MyQTcpSocket *socket)
{
    if (message.data.connectionType == CONNECTION_TYPE_DEMO || message.data.connectionType == CONNECTION_TYPE_RELAY || message.data.connectionType == CONNECTION_TYPE_REPLY) {
        DemoProtocol *protocol = new DemoProtocol(this);
        protocol->setSocket(socket);
        //protocol->setParent(socket);
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

/**
 * @brief Called when a connection to a sliver is established
 */
void DemoCore::connected()
{
    NetworkEntity::connected();
    MyQTcpSocket *socket = (MyQTcpSocket*) sender();
    Sliver *sliver = getSliver(socket);
    setStatus(sliver, Sliver::STATUS_CONNECTED);
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

/**
 * @brief Handles what to do when receiving information that a node is updating
 * @param socket socket of the node connection.
 */
void DemoCore::handleNodeUpdating(MyQTcpSocket *socket)
{
    Sliver *sliver = getSliver(socket);
    setStatus(sliver, Sliver::STATUS_UPDATING);
}

/**
 * @brief Passes a new ping message along by emitting it as a signal.
 * The GUI can then tap into this signal and handle it.
 * @param message Message containing the ping data.
 * @param socket Socket the data was received on.
 */
void DemoCore::handleNewPingReply(PingReply message, MyQTcpSocket *socket)
{
    emit newPingReply(*getSliver(socket), message);
}

/**
 * @brief Passes node info along to the GUI by emitting it as a signal.
 * @param message
 * @param socket
 */
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

/**
 * @brief Passes transfer info along to the GUI by emitting it as a signal.
 * @param message
 * @param socket
 */
void DemoCore::handleNewTransferStatus(TransferStatusMessage message, MyQTcpSocket *socket)
{
    emit newTransferStatus(*getSliver(socket), message);
}

/**
 * @brief Returns the entity type of this NetworkEntity derived class. Used when sending the handshake,
 * which is dealing with the abstract NetworkEntity.
 * @return The type of entity. Either NetworkEntity::ENTITY_TYPE_DEMO or NetworkEntity::ENTITY_TYPE_NODE.
 */
int DemoCore::getEntityType()
{
    return NetworkEntity::ENTITY_TYPE_DEMO;
}

void DemoCore::setStatus(Sliver *sliver, int status)
{
    sliver->setStatus(status);
    emit nodeStatusChanged(sliver->hostName);
}

