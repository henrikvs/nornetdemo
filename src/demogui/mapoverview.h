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
#include "graphdata.h"
#include "nodemodel.h"

class QCustomPlot;
class QCPGraph;

namespace Ui {
class MapOverview;
}

struct NodeStruct {
   QMap<int, QStringList> providers;
   NodeInfoMessage nodeInfo;
   Sliver sliver;
   QStringList ipv4Adrses;
   QStringList ipv6Adrses;
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
    void disconnectFromNodes();
    void applySettings();
    void addGraphData(QString id, qreal data);
    void showGraph(QString id);
    void addNodesToTable();
    NodeModel nodeModel;

    QCustomPlot *createGraph(QString label);

    Ui::MapOverview *ui;
    NodeMapWidget *gmap;

    QList <PlotWindow*> plotWindows;
    QStringList nodesDealtWith;
    QHash<QString, NodeStruct> nodeHash;
    QHash<QString, GraphData*> graphHash;
    QHash<QString, QString> ipToProviderIdHash;
    QHash<QString, QString> ipToNodeIdHash;
    QCustomPlot *pingPlot;
    DemoCore core;
    qreal dmsToDecimal(qreal degrees, qreal minutes, qreal seconds, QString dir);
    bool isConnected;
public slots:
    void handleAboutToQuit();
private slots:
    void showNodeInfo(QString nodeId);
    void showProviderInfo(QString nodeId, QString providerIndex);
    void showConnectionInfo(QString connectionId);
    void showMapInfo();

    void handleNewStatusMessage(Sliver sliver, NodeInfoMessage message);
    void handleNewPingReply(Sliver sliver, PingReply message);
    void handleNewTransferStatus(Sliver sliver, TransferStatusMessage message);
    void handleMapLoaded();
    void handleMapHovered();

    void handleNodeDisconnected(Sliver sliver);

    void handleNodeSelected(QString hostname);
    void handleNodeHovered(QString hostname);
    void handleNodeHoveredOff(QString hostname);

    void handleConnectionSelected(QString id);
    void handleConnectionHovered(QString id);
    void handleConnectionHoveredOff(QString id);
    void handleConnectionRightClicked(QString id);

    void handleAddressSelected(QString nodeId, QString address);
    void handleProviderHovered(QString nodeId, QString address);
    void handleProviderHoveredOff(QString nodeId, QString address);

    void handleShutDownComplete(int status);
    
    void handleNewNodeStatus(QString originalName, Sliver sliver);



    void handleRefresh();
    void handleConnectionRequest(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId);
    void on_actionSettings_triggered();
    void on_actionConnect_to_slivers_triggered();
    void on_actionKill_nodes_2_triggered();
    void on_addNodeButton_clicked();
    void on_removeNodeButton_clicked();
};

#endif // MAPOVERVIEW_H
