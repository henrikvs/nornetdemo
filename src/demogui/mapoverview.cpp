#include "mapoverview.h"
#include "ui_mapoverview.h"
#include "democore.h"
#include "connectionselectiondialog.h"
#include <QTreeView>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QSettings>
#include <QCursor>
#include <QPoint>
#include <QRect>
#include <QTime>
#include "qcustomplot.h"
#include "settings.h"
#include "settingsdialog.h"
#include "popupwidget.h"
#include "regexhelper.h"
#include "selecttestdialog.h"
#include "transferrequestmessage.h"
#include "graphdata.h"
#include "plotwindow.h"

/**
 * @brief Sets up the main window, and connects the  GMapWidget instance with the DemoCore instance
 * @param parent
 */
MapOverview::MapOverview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapOverview), isConnected(false)
{

    ui->setupUi(this);

    ui->statusbar->showMessage(tr("Disconnected"));


    ui->nodeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    applySettings();
    addNodesToTable();
    gmap = ui->mapWidget;
    connect(gmap, SIGNAL(mapLoaded()), this, SLOT(handleMapLoaded()));

    gmap->start();
    constants.loadISPsFromFile(":/nornetapi-constants");
    constants.loadSitenamesFromFile(":/nornet_sites");

    //setCentralWidget(gmap);

    connect(ui->experimentsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(experimentItemRightClicked(QListWidgetItem*)));
    connect(ui->connectedList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(handleConnectedNodeClicked(QListWidgetItem*)));

    connect(&core, SIGNAL(nodeStatusChanged(QString)), Settings::sliceManager.getModel(), SLOT(onStatusChanged(QString)));

    connect(&Settings::sliceManager, SIGNAL(newNodeUpdate(QString,Sliver)), this, SLOT(handleNewNodeStatus(QString,Sliver)));
    connect(&core, SIGNAL(shutDownComplete(int)), this, SLOT(handleShutDownComplete(int)));
    //connect(gmap, SIGNAL(markerSelected(QString)), this, SLOT(markerSelected(QString)));

    connect(gmap, SIGNAL(mapHovered()), this, SLOT(handleMapHovered()));
    connect(gmap, SIGNAL(connectionHovered(QString)), this, SLOT(handleConnectionHovered(QString)));
    connect(gmap, SIGNAL(connectionHoveredOff(QString)), this, SLOT(handleConnectionHoveredOff(QString)));
    connect(gmap, SIGNAL(connectionClicked(QString)), this, SLOT(handleConnectionSelected(QString)));

    connect(gmap, SIGNAL(nodeSelected(QString)), this, SLOT(handleNodeSelected(QString)));
    connect(gmap, SIGNAL(nodeHovered(QString)), this, SLOT(handleNodeHovered(QString)));
    connect(gmap, SIGNAL(nodeHoveredOff(QString)), this, SLOT(handleNodeHoveredOff(QString)));

    connect(gmap, SIGNAL(connectionRequest(QString,QString,QString, QString)), SLOT(handleConnectionRequest(QString,QString,QString, QString)));
    connect(gmap, SIGNAL(doRefresh()), this, SLOT(handleRefresh()));

    connect(&core, SIGNAL(newStatusMessage(Sliver,NodeInfoMessage)), this, SLOT(handleNewStatusMessage(Sliver,NodeInfoMessage)));
    connect(&core, SIGNAL(newPingReply(Sliver,PingReply)), this, SLOT(handleNewPingReply(Sliver,PingReply)));
    connect(&core, SIGNAL(newTransferStatus(Sliver,TransferStatusMessage)), this, SLOT(handleNewTransferStatus(Sliver, TransferStatusMessage)));
    connect(&core, SIGNAL(sliverDisonnected(Sliver)), SLOT(handleNodeDisconnected(Sliver)));
    connect(gmap, SIGNAL(providerSelected(QString,QString)), this, SLOT(handleAddressSelected(QString, QString)));
    connect(gmap, SIGNAL(providerHovered(QString,QString)), this, SLOT(handleProviderHovered(QString, QString)));
    connect(gmap, SIGNAL(providerHoveredOff(QString,QString)), this, SLOT(handleProviderHoveredOff(QString, QString)));
    //core.start();
    connect(gmap, SIGNAL(connectionRightClicked(QString)), this, SLOT(handleConnectionRightClicked(QString)));
    connect(gmap, SIGNAL(providerHovered(QString,QString)), this, SLOT(handleProviderHovered(QString, QString)));
    connect(gmap, SIGNAL(providerHoveredOff(QString,QString)), this, SLOT(handleProviderHoveredOff(QString, QString)));

    //connect(gmap, SIGNAL(markerSelected(QString)), this, SLOT(handleNodeSelected(QString)));

}

MapOverview::~MapOverview()
{
    delete ui;
    Settings::sliceManager.writeSliversToFile();

    qDeleteAll(graphHash.values());
}


void MapOverview::addNodesToTable()
{

    ui->nodeView->setModel(Settings::sliceManager.getModel());

}



void MapOverview::handleRefresh()
{
    qDebug() << "Doing refresh";
    gmap->repaint();
}

/**
 * @brief Returns the first Ipv4 address in the QStringList
 * @param adrs A list of IPv4 and ipv6 addresses
 * @return An IPv4 address
 */
QString MapOverview::getIpv4Address(QStringList adrs)
{
    foreach(QString adr, adrs) {
        if (adr.contains(".")) {
            return adr;
        }
    }
    return QString();
}

/**
 * @brief Returns the first Ipv6 address in the QStringList
 * @param adrs A list of IPv4 and Ipv6 addresses
 * @return An IPv6 address
 */
QString MapOverview::getIpv6Address(QStringList adrs)
{
    foreach(QString adr, adrs) {
        if (adr.contains(":")) {
            return adr;
        }
    }
    return QString();
}

/**
 * @brief Connects to all slivers that are not already connected, or in the process of being connected to
 */
void MapOverview::connectToSlivers()
{
    foreach(Sliver *sliver, Settings::sliceManager.getSlivers()) {
        QTime dieTime = QTime::currentTime().addMSecs(100);
        while (QTime::currentTime() < dieTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        if (!nodesDealtWith.contains(sliver->hostName)) {
            core.connectToSlivers(QList<Sliver*>() << sliver);
            nodesDealtWith << sliver->hostName;
            ui->statusbar->showMessage(QString("Connecting"));
        }
    }
}



/**
 * @brief Forcefully shuts down all nodes by connecting via SSH and killing the processes.
 */
void MapOverview::killNodes()
{
    QList<Sliver*> slivers = Settings::sliceManager.getSlivers();
    core.shutDownNodesSSH(slivers);
}

void MapOverview::disconnectFromNodes()
{
    core.disconnectNodes(Settings::sliceManager.getSlivers());
}



/**
 * @brief Applies settings from QSettings to the program. Called at startup, and after closing the preference window
 */
void MapOverview::applySettings()
{
    QSettings settings(Settings::programName, Settings::company);
    bool gatekeeperEnabled = settings.value(Settings::gatekeeperEnabled, false).toBool();
    if (gatekeeperEnabled) {
        QString username = settings.value(Settings::gatekeeperUsername, QString()).toString();
        QString hostname = settings.value(Settings::gatekeeperHostname, QString()).toString();
        if (username.isEmpty() || hostname.isEmpty()) {
            qDebug() << "Gatekeeper hostname or username not defined";
        } else {
            core.enableGatekeeper(username, hostname);
        }
    } else {
        qDebug() << "Gatekeeper not enabled";
    }

    QString nodeprogRootUrl = settings.value(Settings::nodeprogRootUrl, QString()).toString();
    qDebug() << "root url:" << nodeprogRootUrl;
    QString sliceName = settings.value(Settings::sliceName, QString()).toString();
    core.setActiveSlice(sliceName);
    core.setnodeprogRootUrl(nodeprogRootUrl);


    bool relayEnabled = settings.value(Settings::relayEnabled, false).toBool();

    if (relayEnabled) {
        QString relayHostname = settings.value(Settings::relayHostname, QString()).toString();
        QString relayPort = settings.value(Settings::relayPort, QString()).toString();
        qDebug() << "Enabling relay:" << relayHostname << relayPort;
        core.enableRelay(relayHostname, relayPort.toInt());
    }

    /*if (isConnected) { //if we're already connected, we connect any new slivers added from the preference screen
        connectToSlivers();
    }*/

}

/**
 * @brief Adds some data to a graph. Data will either represent Mbps or MS delay
 * @param The id of the graph, this is the same as the connectionId
 * @param Mbps or MS delay
 */
void MapOverview::addGraphData(QString id, qreal data)
{

    double now = QDateTime::currentDateTime().toTime_t();
    graphHash[id]->addData(now, data);
}

/**
 * @brief Used to convert from the degrees, minutes, seconds geo position system to the decimal system. Will be used for both latitud and longitude
 * @param degrees
 * @param minutes
 * @param seconds
 * @return
 */
qreal MapOverview::dmsToDecimal(qreal degrees, qreal minutes, qreal seconds, QString dir)
{
    qreal res = degrees+(minutes/60)+(seconds/3600);
    if (dir == "S" || dir == "W") {
        res = -res;
    }
    return res;
}

/**
 * @brief A slot that is used when a node has connected, and is sending it's information.
 * Will use this information to draw the node on the map
 * @param sliver The sliver that the message is from
 * @param message
 */
void MapOverview::handleNewStatusMessage(Sliver sliver, NodeInfoMessage message)
{
    if (message.data.lat.isEmpty() || message.data.lng.isEmpty()) {
        return;
    }
    qDebug() << "lat: " <<  message.data.lat;
    qDebug() << "lng: " <<  message.data.lng;
    QStringList lat = message.data.lat.split(QRegExp("\\s"));
    QStringList lng = message.data.lng.split(QRegExp("\\s"));
    qreal decLat = dmsToDecimal(lat[0].toDouble(), lat[1].toDouble(), lat[2].toDouble(), lat[3]); //change the location format
    qreal decLng = dmsToDecimal(lng[0].toDouble(), lng[1].toDouble(), lng[2].toDouble(), lng[3]);
    ui->connectedList->addItem(sliver.hostName);

    int sliverCount = Settings::sliceManager.sliverCount();
    int connected = ui->connectedList->count();
    if (connected == sliverCount) {
        ui->statusbar->showMessage("Connected");
    } else {
        ui->statusbar->showMessage(QString("Connected to %1 out of %2 nodes").arg(connected).arg(sliverCount));
    }

    Provider address;
    NetworkInterface networkinterface;



    foreach(NetworkInterface interfac, message.data.interfaces) {
        if (interfac.interfaceName == "eth0") {
            foreach (QString address, interfac.addresses) {
                if (address.startsWith("10.")) {
                    int providerId = RegexHelper::getFirst(address, "^10\.(\\d+)\.").toInt();
                    nodeHash[sliver.hostName].providers[providerId] << address;
                    qDebug() << "Inserting" << address << "with" << providerId;
                    ipToProviderIdHash[address] = QString::number(providerId);
                    ipToNodeIdHash[address] = sliver.hostName;
                } else if (address.startsWith("2001:700:4100")) {
                    QString str = RegexHelper::getFirst(address, "^2001:700:4100:(\\w+):");
                    QString providerIdHex = str.mid(0, str.length() - 2);
                    bool ok;
                    int providerId = providerIdHex.toUInt(&ok, 16);
                    if (!ok) {
                        qDebug() << "failed to convert hex to decimal" << str;
                    } else {
                        nodeHash[sliver.hostName].providers[providerId] << address;
                        qDebug() << "Inserting:" << address << "with" << providerId;
                        ipToProviderIdHash[address] = QString::number(providerId);
                        ipToNodeIdHash[address] = sliver.hostName;
                    }
                } else {
                    qDebug() << "Unknown address: " << address;
                }
                //gmap->addConnectionMarker(sliver.name, address);
            }
            break;
        }
    }

   gmap->addNodeMarker(sliver.hostName,nodeHash[sliver.hostName].providers.size(), decLat, decLng);
   QMapIterator<int, QStringList> i(nodeHash[sliver.hostName].providers);
    while (i.hasNext()) {
        i.next();
        qDebug() << "Adding marker:";
        gmap->addProviderMarker(sliver.hostName, QString::number(i.key()));
    }

    nodeHash[sliver.hostName].nodeInfo = message;
    nodeHash[sliver.hostName].sliver = sliver;
}

/**
 * @brief Handles a new ping message from a node.
 * @param sliver The sliver the message is from
 * @param message
 */
void MapOverview::handleNewPingReply(Sliver sliver, PingReply message)
{
    QString srcProvider = ipToProviderIdHash[message.data.localIp];
    QString srcNode =  ipToNodeIdHash[message.data.localIp];

    QString destProvider = ipToProviderIdHash[message.data.remoteHost];
    QString destNode = ipToNodeIdHash[message.data.remoteHost];

    int sessionId = message.data.sessionId;
    gmap->drawConnectionTraffic(srcNode, srcProvider, destNode, destProvider, QString::number(sessionId));
    QString id = QString::number(sessionId) + ":" + srcProvider + "@" + srcNode + ":" + destProvider + "@" + destNode;
    if (message.data.state == PingReply::STATE_RUNNING) {
        qDebug() << "ping reply: new Id: " << message.data.localIp << message.data.remoteHost << id;
        addGraphData(id, message.data.ms.toDouble());
    } else if (message.data.state == PingReply::STATE_FINISHED) {
        qDebug() << "Finished pinging";
        gmap->removeConnectionLine(srcNode, srcProvider, destNode, destProvider, QString::number(sessionId));
    } else {
        qDebug() << "Unknown state";
    }
    /*if (graphHash.contains(id)) {
        qDebug() << "Getting old";
        graph = graphHash[id].graph;
    } else {
        qDebug() << "Creating new";
        //graph = new QCPGraph(pingPlot->xAxis, pingPlot->yAxis);
        //graph = pingPlot->addGraph();

        graph->setVisible(false);
        graphHash[id] = graph;
    }*/

    /*
    double now = QDateTime::currentDateTime().toTime_t();
    graph->addData(now, message.data.ms.toDouble());
    if (graph->visible()) {
        pingPlot->rescaleAxes(true);
        pingPlot->replot();
    }*/
}

/**
 * @brief Handles a new transfer status, and draws it on the map
 * @param sliver The sliver the message is from
 * @param message
 */
void MapOverview::handleNewTransferStatus(Sliver sliver, TransferStatusMessage message)
{
    QString srcProvider = ipToProviderIdHash[message.data.localIp];
    QString srcNode =  ipToNodeIdHash[message.data.localIp];

    QString destProvider = ipToProviderIdHash[message.data.remoteHost];
    QString destNode = ipToNodeIdHash[message.data.remoteHost];

    int sessionId = message.data.transferId;
    gmap->drawConnectionTraffic(srcNode, srcProvider, destNode, destProvider, QString::number(sessionId));

    QString id = QString::number(sessionId) + ":" + srcProvider + "@" + srcNode + ":" + destProvider + "@" + destNode;
    if (message.data.state == TransferStatusMessage::STATE_RUNNING) {
        qDebug() << "transfer data: new Id: " << message.data.localIp << message.data.remoteHost << id;
        qDebug() << "Transfer speed:" << message.data.mbps;
        addGraphData(id, message.data.mbps.toDouble());
    } else if (message.data.state == TransferStatusMessage::STATE_FINISHED) {
        qDebug() << "Finished the transfer";
        gmap->removeConnectionLine(srcNode, srcProvider, destNode, destProvider, QString::number(sessionId));
    } else {
        qDebug() << "Error: Unknown state";
    }
}



/**
 * @brief Slot that handles things that needs to be done after the map has finished it loading at startup.
 */
void MapOverview::handleMapLoaded()
{
    NodeInfoMessage message;
    message.data.lat = "60.130564";
    message.data.lng = "10.81604";
    NetworkInterface interfaces;
    interfaces.interfaceName = "eth0";
    interfaces.addresses << "192.165.2.1" << "145.554.223.1" << "123.232.23.1";

    message.data.interfaces << interfaces;
    Sliver sliver;
    sliver.hostName = "Test";

    NodeInfoMessage message2;
    message2.data.lat = "60.550564";
    message2.data.lng = "10.11604";
    NetworkInterface interfaces2;
    interfaces2.interfaceName = "eth0";
    interfaces2.addresses << "192.165.2.3" << "145.554.223.2" << "123.232.23.23";

    message2.data.interfaces << interfaces2;
    Sliver sliver2;
    sliver.hostName = "Test2";

    //handleNewStatusMessage(sliver, message);
    //handleNewStatusMessage(sliver2, message2);

   //showPingGraph("192/155/0");
   //showPingGraph("192.165.2.1123.232.23.230");


}


/**
 * @brief Slot that deals with what to do if a node is selected
 * @param The id of the node
 */
void MapOverview::handleNodeSelected(QString id)
{
    qDebug() << "Node is selected";
}

/**
 * @brief Slot that deals with what to do if a node is selected
 * @param The id of the connection
 */
void MapOverview::handleConnectionSelected(QString id)
{
    qDebug() << "Connection selected: " << id;
    qDebug() << "Showing graph";
    showGraph(id);
}

/**
 * @brief Shows the data of the test with id in a graph, usually called after a connection is selected
 * @param id
 */
void MapOverview::showGraph(QString id)
{
    qDebug() << "Showing: " << id;
    GraphData *graph = graphHash[id];
    QCPDataMap &data = graph->data;

    /*if (graphHash.contains(id)) {
        qDebug() << "Getting old";
        graph = graphHash[id];
        //pingPlot->addPlottable(graph);
        graph->setVisible(true);
        pingPlot->replot();
    } else {
        qDebug() << "Creating new plottable";
        graph = pingPlot->addGraph();
        graph->setVisible(true);
        graphHash[id] = graph;
    }*/
    PlotWindow *window = new PlotWindow(this);

    window->setAttribute(Qt::WA_DeleteOnClose);
    graph->bindToWindow(window);
    connect(window, &PlotWindow::destroyed, [this, window]() {
       plotWindows.removeAll(window);
       qDebug() << "Removed plot window";
    });

    // make the splitter more obvious
    QSplitterHandle *handle = ui->splitter->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);



    window->setWindowTitle(window->getName());
    window->show();

    plotWindows << window;
}


/**
 * @brief This slot handles a request to start a new connection experiment between two nodes with specified providers.
 * It will launch a dialog window that prompts the user of what kind of experiment to start(and what kind of connection to use).
 * It will then send the request to the DemoCore instance.
 * A connection request is usually emitted from the QGMapWidget instance.
 *
 * @param srcNodeId
 * @param srcProviderId
 * @param destNodeId
 * @param destProviderId
 */
void MapOverview::handleConnectionRequest(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId)
{

    qDebug() << "new connection request: " << srcNodeId << srcProviderId << destNodeId << destProviderId;
    //ConnectionSelectionDialog *dialog = new ConnectionSelectionDialog(this);
    //dialog->show();
    //QStringList choices;
    //choices << "TCP" << "Ping";
    //bool ok;
    //QString item = QInputDialog::getItem(this, tr("Select connection"), tr("Type"), choices, 0, false, &ok);

    NodeStruct &srcNode = nodeHash[srcNodeId];
    QStringList srcAdrs = srcNode.providers.value(srcProviderId.toInt());

    NodeStruct &destNode = nodeHash[destNodeId];
    QStringList destAdrs = destNode.providers.value(destProviderId.toInt());

    QString srcIpv4Adr = getIpv4Address(srcAdrs);
    QString destIpv4Adr = getIpv4Address(destAdrs);

    QString srcIpv6Adr = getIpv6Address(srcAdrs);
    QString destIpv6Adr = getIpv6Address(destAdrs);

    bool enableIpv4Option = false;
    bool enableIpv6Option= false;
    if (!srcIpv4Adr.isEmpty() && !destIpv4Adr.isEmpty()) {
        enableIpv4Option = true;
    }

    if (!srcIpv6Adr.isEmpty() && !destIpv6Adr.isEmpty()) {
        enableIpv6Option = true;
    }

    qDebug() << "Showing dialog";

    SelectTestDialog dialog(enableIpv4Option, enableIpv6Option, this);
    dialog.exec();

    if (!dialog.isAccepted()) {
        qDebug() << "Some error";
        return;
    }
    int connectionType = dialog.getConnectionType();
    int testType = dialog.getTestType();
    int duration = dialog.getDurationSeconds();
    qDebug() << connectionType << testType;

    QString srcIp;
    QString destIp;
    GraphData::IPType ipType;
    if (connectionType == SelectTestDialog::CONNECTION_IPV4) {
        srcIp = srcIpv4Adr;
        destIp = destIpv4Adr;
        ipType = GraphData::IPv4;
    } else if (connectionType == SelectTestDialog::CONNECTION_IPV6) {
        srcIp = srcIpv6Adr;
        destIp = destIpv6Adr;
        ipType = GraphData::IPv6;
    } else {
        qDebug() << "No selection";
        return;
    }
    QString id;
    qDebug() << "Doing connect";
    if (testType == SelectTestDialog::TEST_TCP) {
        qDebug() << "Doing tcp test:" << srcIp << destIp;
        qDebug() << "Pinging host:" << srcNodeId << srcIp << destIp;
        int sessionId = core.transferRequest(srcNodeId, destIp, srcIp, TransferRequestMessage::TRANSFER_TYPE_TCP,duration);
        gmap->addConnectionLine(srcNodeId, srcProviderId, destNodeId, destProviderId, QString::number(sessionId));
        id = QString::number(sessionId) + ":" + srcProviderId + "@" + srcNodeId + ":" + destProviderId + "@" + destNodeId;
        graphHash[id] = new GraphData(GraphData::TCP, ipType, id, srcIp, destIp, srcNodeId, destNodeId, srcProviderId, destProviderId, sessionId);
        //gmap->addConnectionLine(localIp, remoteIp, QString::number(0));
    } else if(testType == SelectTestDialog::TEST_PING) {
        qDebug() << "Doing ping test:" << srcIp << destIp;
        qDebug() << "Pinging host:" << srcNodeId << srcIp << destIp;
        int sessionId = core.pingHost(srcNodeId, destIp, srcIp, duration);
        gmap->addConnectionLine(srcNodeId, srcProviderId, destNodeId, destProviderId, QString::number(sessionId));
        id = QString::number(sessionId) + ":" + srcProviderId + "@" + srcNodeId + ":" + destProviderId + "@" + destNodeId;
        graphHash[id] = new GraphData(GraphData::PING, ipType, id, srcIp, destIp, srcNodeId, destNodeId, srcProviderId, destProviderId, sessionId);
    } else {
        return;
    }

    GraphData *data = graphHash[id];

    QListWidgetItem *item = new QListWidgetItem(data->expTypeString() + "-" + data->ipTypeString());
    item->setData(Qt::UserRole, id);
    ui->experimentsList->insertItem(0, item);
    //ui->experimentsList->addItem(item);

    qDebug() << "connection type" << connectionType << "test type" << testType;
    /*if (ok && !item.isEmpty()) {
        qDebug() << "Successful select";
        if (item == "TCP") {
            core.pingHost(nodeId, localIp, remoteIp);
            gmap->addConnectionLine(localIp, remoteIp, QString::number(0));
        } else if(item == "Ping") {
            gmap->addConnectionLine(localIp, remoteIp, QString::number(0));
            //core->
        }
    } else {
        //gmap->removeConnectionLine(localIp, remoteIp);
    }*/

}

void MapOverview::handleConnectionHovered(QString connectionId)
{
    showConnectionInfo(connectionId);

    //PopupWidget *popupWidget = new PopupWidget(address, this);
    //QLabel *label = new QLabel(this);
    //label->setText(address);
    //label->adjustSize();
    //label->setWindowFlags(Qt::Popup);
    //QPoint point = QCursor::pos();
    //popupWidget->move(point);
    //popupWidget->show();
    //gmap->setFocus();
}

void MapOverview::handleConnectionHoveredOff(QString address)
{
    qDebug() << "connection hovered off";
        //ui->stackedWidget->setCurrentWidget(ui->mapPage);
}

/**
 * @brief Handler for right clicking events on connections from the user.
 * It will open a popup, asking the user to either remove the connection, or add it to an existing (or new) graph window
 * @param id
 */
void MapOverview::handleConnectionRightClicked(QString id)
{

    showExperimentMenu(id);
    qDebug() << "Connectino was right clicked in mapoverview";
}

void MapOverview::showExperimentMenu(QString id) {
    QMenu menu(this);
    QPoint point = QCursor::pos();
    GraphData *graphData = graphHash[id];

    QMenu *graphMenu = menu.addMenu("Add to graph");
    QAction *addaction = graphMenu->addAction("New Plot window");
    connect(addaction, &QAction::triggered, [this, id]() {
       showGraph(id);
    });
    foreach(PlotWindow* window, plotWindows) {
        QAction *action = graphMenu->addAction(window->getName());
        connect(action, &QAction::triggered, [window, &graphData]() {
            graphData->bindToWindow(window);
        });
    }
    QAction *action = menu.addAction("Remove");
    connect(action, &QAction::triggered, [this, graphData]() {
        gmap->removeConnectionLine(graphData->getSrcNode(), graphData->getSrcProviderId(), graphData->getDestNode(), graphData->getDestProviderId(), QString::number(graphData->getsessionId()));
        core.stopExperiment(graphData->getSrcNode(), graphData->getsessionId());
       qDebug() << "Triggered remove";
    });

    QAction *selectedItem = menu.exec(point);
}

void MapOverview::handleAddressSelected(QString nodeId, QString address)
{
    qDebug() << " Address selected" << nodeId << address;
}

/**
 * @brief If a provider connection was hovered, we show the providerinfo widget in the sidebar.
 * @param nodeId
 * @param The provider index
 */
void MapOverview::handleProviderHovered(QString nodeId, QString providerIndex)
{
    showProviderInfo(nodeId, providerIndex);
}

void MapOverview::handleProviderHoveredOff(QString nodeId, QString address)
{
    qDebug() << " Address hovered off";
        //ui->stackedWidget->setCurrentWidget(ui->mapPage);
}

void MapOverview::handleShutDownComplete(int status)
{
    qDebug() << "Shut down complete";
    
}

void MapOverview::handleNewNodeStatus(QString originalName, Sliver sliver)
{
   qDebug() << "New name here";
}

void MapOverview::handleAboutToQuit()
{
    foreach(PlotWindow *window, plotWindows) {
        window->close();
        qDebug() << "Removing window";
    }
    core.shutDown();
    gmap->deleteLater();
    connect(gmap, &MapWidget::destroyed, []() {
        qDebug() << "Gmap destroyed";
    });
    qDebug() << "shutting down core";
}

void MapOverview::experimentItemRightClicked(QListWidgetItem *item)
{
    qDebug() << "Double clicked item ";
    showExperimentMenu(item->data(Qt::UserRole).toString());
}

void MapOverview::handleConnectedNodeClicked(QListWidgetItem *item)
{
    showNodeInfo(item->text());
    gmap->panToNode(item->text());
}

void MapOverview::showNodeInfo(QString nodeId)
{
    //QToolTip::showText(point, hostname,gmap, QRect(point, QSize(10,10)));
    NodeStruct &node = nodeHash[nodeId];
    QString siteAbbr = getSiteAbbr(node.sliver.hostName);
    ui->stackedWidget->setCurrentWidget(ui->nodePage);
    ui->sliceNameEdit->setText(constants.getFullSiteName(siteAbbr));
    ui->siteNameEdit->setText(node.sliver.hostName);
    ui->sliceNameEdit->setCursorPosition(0);
    ui->siteNameEdit->setCursorPosition(0);
    qDebug() << "Node hovered";
}

void MapOverview::showProviderInfo(QString nodeId, QString providerIndex)
{
    ui->stackedWidget->setCurrentWidget(ui->providerPage);
    ui->providerName->setText(constants.getFullISPName(providerIndex.toInt()));
    ui->providerName->setCursorPosition(0);

    NodeStruct &srcNode = nodeHash[nodeId];
    QStringList srcAdrs = srcNode.providers.value(providerIndex.toInt());
    ui->addressListWidget->clear();
    ui->addressListWidget->addItems(srcAdrs);
    qDebug() << " Address hovered" << nodeId << providerIndex;
}

void MapOverview::showConnectionInfo(QString connectionId)
{
    QStringList splitted = connectionId.split("@");
    QString provider = splitted[0];
    QString site = splitted[1];
    GraphData *graph = graphHash[connectionId];
    qDebug() << "Connection hovered" << connectionId;
    ui->stackedWidget->setCurrentWidget(ui->connectionPage);
    ui->experimentEdit->setText(graph->expTypeString());
    ui->ipEdit->setText(graph->ipTypeString());
    ui->srcAddrEdit->setText(graph->getSrcAddr());
    ui->destAddrEdit->setText(graph->getDestAddr());
}

void MapOverview::showMapInfo()
{

}

void MapOverview::handleMapHovered()
{
//ui->stackedWidget->setCurrentWidget(ui->mapPage);
}

/**
 * @brief What to do on the GUI frontent if node is disconnected
 * It basically just updates the GUI, and allows the connect button to be repressed, so that the user can try to reconnect.
 * @param sliver
 */
void MapOverview::handleNodeDisconnected(Sliver sliver)
{
    qDebug() << "Problem connecting to " << sliver.hostName << sliver.IPv6 << sliver.port;
    nodesDealtWith.removeAll(sliver.hostName);
    QList<QListWidgetItem*> items = ui->connectedList->findItems(sliver.hostName, Qt::MatchExactly);
    qDeleteAll(items);

    gmap->removeNodeMarker(sliver.hostName);
    nodeHash.remove(sliver.hostName);
    int sliverCount = Settings::sliceManager.sliverCount();
    int connected = ui->connectedList->count();
    ui->statusbar->showMessage(QString("Connected to %1 out of %2 nodes").arg(connected).arg(sliverCount));
    ui->actionConnect_to_slivers->setEnabled(true);
}

void MapOverview::handleNodeHovered(QString hostname)
{
    showNodeInfo(hostname);
}

void MapOverview::handleNodeHoveredOff(QString hostname)
{
    qDebug() << "Node hovered off";
        //ui->stackedWidget->setCurrentWidget(ui->mapPage);
}

/**
 * @brief Will create an empty graph that can be drawn in a window.
 * @param label
 * The label of the Y axis. (The X axis will always be time)
 * @return
 */
QCustomPlot *MapOverview::createGraph(QString label)
{
    double now = QDateTime::currentDateTime().toTime_t();
    QVector<double> y;
    QVector<double> x;

    x.append(42);
    x.append(23);

    y.append(4);
    y.append(7);
    QCustomPlot *customPlot = new QCustomPlot(this);
    //customPlot->addGraph();
    //customPlot->graph(0)->setData(x,y);
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel(label);
    customPlot->xAxis->setDateTimeFormat("mm:ss");
    //customPlot->yAxis->setRange(0,100);
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setRange(now,now + 43);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->setMinimumHeight(150);
    customPlot->setMaximumHeight(200);
    return customPlot;
}

QString MapOverview::getSiteAbbr(QString nodeName)
{
    QRegularExpression re("\\.([^.]+)\\.nornet");
    QRegularExpressionMatch match = re.match(nodeName);
    if (match.hasMatch()) {
        return match.captured(1);
    } else {
        return "site";
    }
}

/**
 * @brief Opens the settings window
 */
void MapOverview::on_actionSettings_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->exec();
    Settings::sliceManager.writeSliversToFile();
    applySettings();
}

/**
 * @brief Connects to slivers
 */
void MapOverview::on_actionConnect_to_slivers_triggered()
{
    isConnected = true;
    ui->actionConnect_to_slivers->setEnabled(false);
    ui->actionKill_nodes_2->setEnabled(true);
    connectToSlivers();
}

void MapOverview::on_actionKill_nodes_2_triggered()
{
    isConnected = false;
    ui->actionConnect_to_slivers->setEnabled(true);
    ui->actionKill_nodes_2->setEnabled(false);
    disconnectFromNodes();
    qDebug() << "Killing nodes";
}

void MapOverview::on_addNodeButton_clicked()
{
    bool ok;
    QString hostname = QInputDialog::getText(this, tr("Settings"), tr("Enter hostname"),QLineEdit::Normal,QString(), &ok);

    if (!ok) return;

    if (!Settings::sliceManager.sliverExists(hostname)) {
        qDebug() << "host does not exist, adding";
        Settings::sliceManager.createSliver(hostname, NetworkEntity::PORT);
        ui->actionConnect_to_slivers->setEnabled(true);
    } else {
        qDebug() << "Host already exists";
    }
}

void MapOverview::on_removeNodeButton_clicked()
{
    QModelIndexList selectedList = ui->nodeView->selectionModel()->selectedRows();
    for (int i = 0; i < selectedList.count(); i++) {
        qDebug() << "Removing row";
        QModelIndex index = selectedList.at(i);
        Sliver *sliver = Settings::sliceManager.getModel()->getNode(index.row());
        Settings::sliceManager.getModel()->removeRows(index.row(), 1, QModelIndex());
        nodesDealtWith.removeAll(sliver->hostName);
        core.disconnectNodes(QList<Sliver*>() << sliver);
    }
}

void MapOverview::on_importButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import file");
    Settings::sliceManager.importNodesFromFile(fileName);
}

void MapOverview::on_killButton_clicked()
{
    core.shutDownNodesSSH(Settings::sliceManager.getSlivers());
}
