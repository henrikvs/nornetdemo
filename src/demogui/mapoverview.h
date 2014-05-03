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

class QCustomPlot;
class QCPGraph;

namespace Ui {
class MapOverview;
}

class MapOverview : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapOverview(QWidget *parent = 0);
    ~MapOverview();

private:
    void connectToSlivers();
    void applySettings();
    Ui::MapOverview *ui;
    NodeMapWidget *gmap;
    QHash<QString, NodeInfoMessage> nodeHash;
    QHash<QString, QCPGraph*> graphHash;
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

    void showPingGraph(QString id);
    void removePingGraph(QString localAddress, QString remoteHost);
    void doRefresh();
    void handleConnectionRequest(QString nodeId, QString localIp, QString remoteIp);
    QWidget *createNodeInfoBox();
    QCustomPlot *createSpeedGraph();
    QCustomPlot *createGraph(QString label);
    void on_actionSettings_triggered();
    void on_actionConnect_to_slivers_triggered();
};

#endif // MAPOVERVIEW_H
