#ifndef MAPOVERVIEW_H
#define MAPOVERVIEW_H

#include <QMainWindow>
#include "sliver.h"
#include "nodeinfomessage.h"
#include "gmapwidget.h"
#include "nodemapwidget.h"
#include "nodestatusmessage.h"
#include "pingreply.h"
#include "democore.h"
#include "qcustomplot.h"

class QCustomPlot;
class QCPGraph;

namespace Ui {
class MapOverview;
}

struct NodeStruct {
   QMap<int, QStringList> providers;
   NodeInfoMessage nodeInfo;
};

/**
 * @brief The Graph struct is used for holding a list of which plots the graph is shown in,
 * so that these can be replotted when there's new data for the graph
 */
struct Graph {
    Graph(int type):type(type) {};
    Graph(){};
    QList<QCustomPlot*> plots;
    QCPDataMap data;
    int type;
};

/**
 * @brief
 * The main window of the NorNet demonstration program.
 *
 * It connects to a GMapWidget instance, and a DemoCore instance, and relays communication data and user interaction data between them.
 * Also deals with graphs.
 */
class MapOverview : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapOverview(QWidget *parent = 0);

    static const int GRAPH_TYPE_PING = 1;
    static const int GRAPH_TYPE_TCP = 2;
    ~MapOverview();

private:
    QString getIpv4Address(QStringList adrs);
    QString getIpv6Address(QStringList adrs);
    void connectToSlivers();
    void killNodes();
    void applySettings();
    void addGraphData(QString id, qreal data);
    Ui::MapOverview *ui;
    NodeMapWidget *gmap;
    QHash<QString, NodeStruct> nodeHash;
    QHash<QString, Graph> graphHash;
    QHash<QString, QString> ipToProviderIdHash;
    QHash<QString, QString> ipToNodeIdHash;
    QCustomPlot *pingPlot;
    DemoCore core;
    qreal dmsToDecimal(qreal degrees, qreal minutes, qreal seconds);

private slots:
    void handleNewStatusMessage(Sliver sliver, NodeInfoMessage message);
    void handleNewPingReply(Sliver sliver, PingReply message);
    void handleNewTransferStatus(Sliver sliver, TransferStatusMessage message);
    void handleMapLoaded();

    void handleNodeSelected(QString hostname);
    void handleNodeHovered(QString hostname);
    void handleNodeHoveredOff(QString hostname);

    void handleConnectionSelected(QString id);
    void handleConnectionHovered(QString id);
    void handleConnectionHoveredOff(QString id);

    void handleAddressSelected(QString nodeId, QString address);
    void handleAddressHovered(QString nodeId, QString address);
    void handleAddressHoveredOff(QString nodeId, QString address);

    void showGraph(QString id);
    void removePingGraph(QString localAddress, QString remoteHost);
    void doRefresh();
    void handleConnectionRequest(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId);
    QWidget *createNodeInfoBox();
    QCustomPlot *createGraph(QString label);
    void on_actionSettings_triggered();
    void on_actionConnect_to_slivers_triggered();
    void on_actionKill_nodes_2_triggered();
};

#endif // MAPOVERVIEW_H
