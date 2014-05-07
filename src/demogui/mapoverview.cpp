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
#include "qcustomplot.h"
#include "settings.h"
#include "settingsdialog.h"
#include "popupwidget.h"
#include "regexhelper.h"
#include "selecttestdialog.h"
#include "transferrequestmessage.h"

/**
 * @brief Sets up the window, and connects the  GMapWidget instance with the DemoCore instance
 * @param parent
 */
MapOverview::MapOverview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapOverview)
{

    ui->setupUi(this);

    applySettings();

    gmap = new NodeMapWidget();
    connect(gmap, SIGNAL(mapLoaded()), this, SLOT(handleMapLoaded()));

    gmap->start();
    //setCentralWidget(gmap);

    pingPlot = createGraph("mbps");
    QSplitter *splitter = new QSplitter(this);

    splitter->addWidget(gmap);
    QVBoxLayout *sideWindowLayout = new QVBoxLayout;
    sideWindowLayout->addWidget(createNodeInfoBox());


    QWidget *sideWindow = new QWidget(this);
    sideWindow->setLayout(sideWindowLayout);
    sideWindowLayout->addWidget(pingPlot);
    QCustomPlot *speedPlot = this->createGraph("Test");
    sideWindowLayout->addWidget(speedPlot);
    splitter->addWidget(sideWindow);
    setCentralWidget(splitter);
    //connect(gmap, SIGNAL(markerSelected(QString)), this, SLOT(markerSelected(QString)));
    connect(gmap, SIGNAL(connectionClicked(QString)), this, SLOT(handleConnectionSelected(QString)));
    connect(gmap, SIGNAL(connectionHovered(QString)), this, SLOT(handleConnectionHovered(QString)));
    connect(gmap, SIGNAL(connectionHoveredOff(QString)), this, SLOT(handleConnectionHoveredOff(QString)));

    connect(gmap, SIGNAL(nodeSelected(QString)), this, SLOT(handleNodeSelected(QString)));
    connect(gmap, SIGNAL(nodeHovered(QString)), this, SLOT(handleNodeHovered(QString)));
    connect(gmap, SIGNAL(nodeHoveredOff(QString)), this, SLOT(handleNodeHoveredOff(QString)));

    connect(gmap, SIGNAL(connectionRequest(QString,QString,QString, QString)), SLOT(handleConnectionRequest(QString,QString,QString, QString)));
    connect(gmap, SIGNAL(doRefresh()), this, SLOT(doRefresh()));

    connect(&core, SIGNAL(newStatusMessage(Sliver,NodeInfoMessage)), this, SLOT(handleNewStatusMessage(Sliver,NodeInfoMessage)));
    connect(&core, SIGNAL(newPingReply(Sliver,PingReply)), this, SLOT(handleNewPingReply(Sliver,PingReply)));
    connect(&core, SIGNAL(newTransferStatus(Sliver,TransferStatusMessage)), this, SLOT(handleNewTransferStatus(Sliver, TransferStatusMessage)));

    connect(gmap, SIGNAL(providerSelected(QString,QString)), this, SLOT(handleAddressSelected(QString, QString)));
    connect(gmap, SIGNAL(providerHovered(QString,QString)), this, SLOT(handleAddressHovered(QString, QString)));
    connect(gmap, SIGNAL(providerHoveredOff(QString,QString)), this, SLOT(handleAddressHoveredOff(QString, QString)));
    //core.start();

    //connect(gmap, SIGNAL(markerSelected(QString)), this, SLOT(handleNodeSelected(QString)));
}

MapOverview::~MapOverview()
{
    delete ui;
    Settings::sliceManager.writeSliversToFile();
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

void MapOverview::connectToSlivers()
{
    QList<Sliver*> slivers = Settings::sliceManager.getSlivers();
    core.connectToSlivers(slivers);
}

void MapOverview::killNodes()
{
    QList<Sliver*> slivers = Settings::sliceManager.getSlivers();
    core.shutDownNodeprogs(slivers);
}



/**
 * @brief Applies settings from QSettings to the program. Called at startup, and after closing the preference window
 */
void MapOverview::applySettings()
{
    Settings::sliceManager.readSliversFromFile();
    QSettings settings(Settings::programName, Settings::company);
    bool gatekeeperEnabled = settings.value(Settings::gatekeeperEnabled, false).toBool();
    if (gatekeeperEnabled) {
        QString username = settings.value(Settings::gatekeeperUsername, QString()).toString();
        QString hostname = settings.value(Settings::gatekeeperHostname, QString()).toString();
        if (username.isEmpty() || hostname.isEmpty()) {
            qDebug() << "Gatekeeper hostname or username not defined";
        } else {
            core.enableGatekeeper("henrik", "gatekeeper.nntb.no");
        }
    } else {
        qDebug() << "Gatekeeper not enabled";
    }


    bool relayEnabled = settings.value(Settings::relayEnabled, false).toBool();

    if (relayEnabled) {
        QString relayHostname = settings.value(Settings::relayHostname, QString()).toString();
        QString relayPort = settings.value(Settings::relayPort, QString()).toString();
        qDebug() << "Enabling relay:" << relayHostname << relayPort;
        core.enableRelay(relayHostname, relayPort.toInt());
    }
}

void MapOverview::addGraphData(QString id, qreal data)
{
    QCPDataMap &dataMap = graphHash[id].data;
    double now = QDateTime::currentDateTime().toTime_t();
    dataMap[now] = QCPData(now, data);
    qDebug() << "datasize: " << dataMap.size();
    foreach(QCustomPlot *plot, graphHash[id].plots) {
        qDebug() << "Replotting";
        //->graph()->setData(&data, false);
        plot->rescaleAxes(false);
        plot->replot();
    }
}

/**
 * @brief Used to convert from the degrees, minutes, seconds geo position system to the decimal system. Will be used for both latitud and longitude
 * @param degrees
 * @param minutes
 * @param seconds
 * @return
 */
qreal MapOverview::dmsToDecimal(qreal degrees, qreal minutes, qreal seconds)
{
    return degrees+(minutes/60)+(seconds/3600);
}

/**
 * @brief A slot that is used when a node has connected, and is sending it's information.
 * Will use this information to draw the node on the map
 * @param sliver The sliver that the message is from
 * @param message
 */
void MapOverview::handleNewStatusMessage(Sliver sliver, NodeInfoMessage message)
{
    qDebug() << "lat: " <<  message.data.lat;
    qDebug() << "lng: " <<  message.data.lng;
    QStringList lat = message.data.lat.split(QRegExp("\\s"));
    QStringList lng = message.data.lng.split(QRegExp("\\s"));
    qreal decLat = dmsToDecimal(lat[0].toDouble(), lat[1].toDouble(), lat[2].toDouble()); //change the location format
    qreal decLng = dmsToDecimal(lng[0].toDouble(), lng[1].toDouble(), lng[2].toDouble());
    gmap->addNodeMarker(sliver.name, decLat, decLng);
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
                    QString str = RegexHelper::getFirst(address, "^2001:700:4100:(\\d+):");
                    QString providerIdHex = str.mid(0, str.length() - 2);
                    bool ok;
                    int providerId = providerIdHex.toUInt(&ok, 16);
                    if (!ok) {
                        qDebug() << "failed to convert hex to decimal";
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
   QMapIterator<int, QStringList> i(nodeHash[sliver.hostName].providers);
    while (i.hasNext()) {
        i.next();
        qDebug() << "Adding marker:";
        gmap->addProviderMarker(sliver.name, QString::number(i.key()));
    }

    nodeHash[sliver.name].nodeInfo = message;

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
    sliver.name = "Test";

    NodeInfoMessage message2;
    message2.data.lat = "60.550564";
    message2.data.lng = "10.11604";
    NetworkInterface interfaces2;
    interfaces2.interfaceName = "eth0";
    interfaces2.addresses << "192.165.2.3" << "145.554.223.2" << "123.232.23.23";

    message2.data.interfaces << interfaces2;
    Sliver sliver2;
    sliver.name = "Test2";

    //handleNewStatusMessage(sliver, message);
    //handleNewStatusMessage(sliver2, message2);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        //this->handleNewPingReply(Sliver(), PingReply("3", "192.165.2.1", "123.232.23.23", 0));
        //this->handleNewPingReply(Sliver(), PingReply("2", "192", "155", 1));
    });
    timer->start(1000);

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
    Graph &graph = graphHash[id];
    QCPDataMap &data = graph.data;
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
    QDialog *dialog = new QDialog(this);
    dialog->setMinimumSize(400,400);
    if (graph.type == GRAPH_TYPE_PING) {
        qDebug() << "showing ping graph";
        QCustomPlot *plot = createGraph("ms");
        QCPGraph *newGraph = plot->addGraph();
        newGraph->setData(&data,false);
        graph.plots << plot;
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        layout->addWidget(plot);
        dialog->setLayout(layout);
        dialog->show();
    } else if (graph.type == GRAPH_TYPE_TCP) {
        qDebug() << "Showing  tcp graph";
        QCustomPlot *plot = createGraph("mbps");
        QCPGraph *newGraph = plot->addGraph();
        newGraph->setData(&data,false);
        graph.plots << plot;
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        layout->addWidget(plot);
        dialog->setLayout(layout);
        dialog->show();
    } else {
        qDebug() << "Error: No recognized graph type";
    }
}

void MapOverview::removePingGraph(QString localAddress, QString remoteHost)
{
    /*QString id = localAddress + "/" + remoteHost;
    QCPGraph *graph;
    if (graphHash.contains(id)) {
        qDebug() << "Removing";
        graph = graphHash[id];
        //pingPlot->removeGraph(graph);
        graph->setVisible(false);
        pingPlot->replot();
    } else {
        qDebug() << "Graph doesn't exist";
    }*/
}

void MapOverview::doRefresh()
{
    qDebug() << "Doing refresh";
    gmap->repaint();
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

    int connectionType = dialog.getConnectionType();
    int testType = dialog.getTestType();
    qDebug() << connectionType << testType;

    QString srcIp;
    QString destIp;
    if (connectionType == SelectTestDialog::CONNECTION_IPV4) {
        srcIp = srcIpv4Adr;
        destIp = destIpv4Adr;
    } else if (connectionType == SelectTestDialog::CONNECTION_IPV6) {
        srcIp = srcIpv6Adr;
        destIp = destIpv6Adr;
    } else {
        qDebug() << "No selection";
    }
    qDebug() << "Doing connect";
    if (testType == SelectTestDialog::TEST_TCP) {
        qDebug() << "Doing tcp test:" << srcIp << destIp;
        qDebug() << "Pinging host:" << srcNodeId << srcIp << destIp;
        int sessionId = core.transferRequest(srcNodeId, destIp, srcIp, TransferRequestMessage::TRANSFER_TYPE_TCP,5);
        gmap->addConnectionLine(srcNodeId, srcProviderId, destNodeId, destProviderId, QString::number(sessionId));
        QString id = QString::number(sessionId) + ":" + srcProviderId + "@" + srcNodeId + ":" + destProviderId + "@" + destNodeId;
        graphHash[id] = Graph(GRAPH_TYPE_TCP);
        //gmap->addConnectionLine(localIp, remoteIp, QString::number(0));
    } else if(testType == SelectTestDialog::TEST_PING) {
        qDebug() << "Doing ping test:" << srcIp << destIp;
        qDebug() << "Pinging host:" << srcNodeId << srcIp << destIp;
        int sessionId = core.pingHost(srcNodeId, destIp, srcIp);;
        gmap->addConnectionLine(srcNodeId, srcProviderId, destNodeId, destProviderId, QString::number(sessionId));
        QString id = QString::number(sessionId) + ":" + srcProviderId + "@" + srcNodeId + ":" + destProviderId + "@" + destNodeId;
        graphHash[id] = Graph(GRAPH_TYPE_PING);
    }
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

void MapOverview::handleConnectionHovered(QString address)
{
    qDebug() << "Connection hovered" << address;
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
}

void MapOverview::handleAddressSelected(QString nodeId, QString address)
{
    qDebug() << " Address selected" << nodeId << address;
}

void MapOverview::handleAddressHovered(QString nodeId, QString address)
{
    qDebug() << " Address hovered" << nodeId << address;
}

void MapOverview::handleAddressHoveredOff(QString nodeId, QString address)
{
    qDebug() << " Address hovered off";
}


void MapOverview::handleNodeHovered(QString hostname)
{
    qDebug() << "Node hovered";
}

void MapOverview::handleNodeHoveredOff(QString hostname)
{
    qDebug() << "Node hovered off";
}

QWidget *MapOverview::createNodeInfoBox()
{
    QGroupBox *box = new QGroupBox("Node", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Sliver", this);
    QTextEdit *edit = new QTextEdit("text here", this);
    layout->addWidget(label);
    layout->addWidget(edit);

    box->setLayout(layout);
    return box;
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

/**
 * @brief Opens the settings window
 */
void MapOverview::on_actionSettings_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->exec();
    Settings::sliceManager.writeSliversToFile();
    applySettings();
}

/**
 * @brief Connects to slivers
 */
void MapOverview::on_actionConnect_to_slivers_triggered()
{
    connectToSlivers();
}

void MapOverview::on_actionKill_nodes_2_triggered()
{
    killNodes();
    qDebug() << "Killing nodes";
}
