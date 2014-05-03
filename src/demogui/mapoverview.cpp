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

    pingPlot = createSpeedGraph();
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

    connect(gmap, SIGNAL(connectionRequest(QString,QString,QString)), SLOT(handleConnectionRequest(QString,QString,QString)));
    connect(gmap, SIGNAL(doRefresh()), this, SLOT(doRefresh()));

    connect(&core, SIGNAL(newStatusMessage(Sliver,NodeInfoMessage)), this, SLOT(handleNewStatusMessage(Sliver,NodeInfoMessage)));
    connect(&core, SIGNAL(newPingReply(Sliver,PingReply)), this, SLOT(handleNewPingReply(Sliver,PingReply)));
    connect(&core, SIGNAL(newTransferStatus(Sliver,TransferStatusMessage)), this, SLOT(handleNewTransferStatus(Sliver, TransferStatusMessage)));

    connect(gmap, SIGNAL(addressSelected(QString,QString)), this, SLOT(handleAddressSelected(QString, QString)));
    connect(gmap, SIGNAL(addressHovered(QString,QString)), this, SLOT(handleAddressHovered(QString, QString)));
    connect(gmap, SIGNAL(addressHoveredOff(QString,QString)), this, SLOT(handleAddressHoveredOff(QString, QString)));
    //core.start();

    //connect(gmap, SIGNAL(markerSelected(QString)), this, SLOT(handleNodeSelected(QString)));
}

MapOverview::~MapOverview()
{
    delete ui;
    Settings::sliceManager.writeSliversToFile();
}

void MapOverview::connectToSlivers()
{
    QList<Sliver*> slivers = Settings::sliceManager.getSlivers();
    core.connectToSlivers(slivers);
}

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

}

qreal MapOverview::dmsToDecimal(qreal degrees, qreal minutes, qreal seconds)
{
    return degrees+(minutes/60)+(seconds/3600);
}

void MapOverview::handleNewStatusMessage(Sliver sliver, NodeInfoMessage message)
{
    qDebug() << "lat: " <<  message.data.lat;
    qDebug() << "lng: " <<  message.data.lng;
    QStringList lat = message.data.lat.split(QRegExp("\\s"));
    QStringList lng = message.data.lng.split(QRegExp("\\s"));
    qreal decLat = dmsToDecimal(lat[0].toDouble(), lat[1].toDouble(), lat[2].toDouble()); //change the location format
    qreal decLng = dmsToDecimal(lng[0].toDouble(), lng[1].toDouble(), lng[2].toDouble());
    gmap->addNodeMarker(sliver.name, decLat, decLng);
    Address address;
    NetworkInterface networkinterface;




    foreach(NetworkInterface interfac, message.data.interfaces) {
        if (interfac.interfaceName == "eth0") {
            foreach (QString address, interfac.addresses) {
                gmap->addConnectionMarker(sliver.name, address);
            }
            break;
        }
    }
    nodeHash[sliver.name] = message;

}

void MapOverview::handleNewPingReply(Sliver sliver, PingReply message)
{
    QString id = message.data.localIp + message.data.remoteHost + QString::number(message.data.sessionId);
    qDebug() << "ping reply: new Id: " << id;
    QCPGraph *graph;
    if (graphHash.contains(id)) {
        qDebug() << "Getting old";
        graph = graphHash[id];
    } else {
        qDebug() << "Creating new";
        //graph = new QCPGraph(pingPlot->xAxis, pingPlot->yAxis);
        graph = pingPlot->addGraph();
        graph->setVisible(false);
        graphHash[id] = graph;
    }

    double now = QDateTime::currentDateTime().toTime_t();
    graph->addData(now, message.data.ms.toDouble());
    if (graph->visible()) {
        pingPlot->rescaleAxes(true);
        pingPlot->replot();
    }
}

void MapOverview::handleNewTransferStatus(Sliver sliver, TransferStatusMessage message)
{
    qDebug() << "New transfer status received";
}



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

void MapOverview::handleNodeSelected(QString id)
{
    qDebug() << "Node is selected";
}

void MapOverview::handleConnectionSelected(QString id)
{
    qDebug() << "Connection selected: " << id;
    qDebug() << "Showing graph";
    showPingGraph(id);
}

void MapOverview::showPingGraph(QString id)
{
    qDebug() << "Showing: " << id;
    QCPGraph *graph;
    if (graphHash.contains(id)) {
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
    }
}

void MapOverview::removePingGraph(QString localAddress, QString remoteHost)
{
    QString id = localAddress + "/" + remoteHost;
    QCPGraph *graph;
    if (graphHash.contains(id)) {
        qDebug() << "Removing";
        graph = graphHash[id];
        //pingPlot->removeGraph(graph);
        graph->setVisible(false);
        pingPlot->replot();
    } else {
        qDebug() << "Graph doesn't exist";
    }
}

void MapOverview::doRefresh()
{
    qDebug() << "Doing refresh";
    gmap->repaint();
}

void MapOverview::handleConnectionRequest(QString nodeId, QString localIp, QString remoteIp)
{
    qDebug() << "new connection request: " << nodeId << localIp << remoteIp;
    //ConnectionSelectionDialog *dialog = new ConnectionSelectionDialog(this);
    //dialog->show();
    QStringList choices;
    choices << "TCP" << "Ping";
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Select connection"), tr("Type"), choices, 0, false, &ok);
    if (ok && !item.isEmpty()) {
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
    }
}

void MapOverview::handleConnectionHovered(QString address)
{
    qDebug() << "Connection hovered";
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
    qDebug() << " Address selected";
}

void MapOverview::handleAddressHovered(QString nodeId, QString address)
{
    qDebug() << " Address hovered";
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

QCustomPlot *MapOverview::createSpeedGraph()
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
    customPlot->yAxis->setLabel("Ping");
    customPlot->xAxis->setDateTimeFormat("mm:ss");
    //customPlot->yAxis->setRange(0,100);
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setRange(now,now + 43);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->setMinimumHeight(150);
    customPlot->setMaximumHeight(200);
    return customPlot;
}

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

void MapOverview::on_actionSettings_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->exec();
}

void MapOverview::on_actionConnect_to_slivers_triggered()
{
    connectToSlivers();
}
