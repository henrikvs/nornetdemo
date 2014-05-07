#ifndef NODEMAPWIDGET_H
#define NODEMAPWIDGET_H

#include "gmapwidget.h"
#include <QHash>


struct Provider {
    qreal offsetX;
    qreal offsetY;
    QString name;
    QString nodeName;
    QString id;
    int connections;
    //QList<Address> connections;
    Provider() {

    }
    Provider(QString name, QString nodeName, QString id, qreal offsetX, qreal offsetY): name(name), nodeName(nodeName),
        id(id), offsetX(offsetX), offsetY(offsetY), connections(0) {

    }
};

struct Node {
    QString name;
    qreal lat;
    qreal lng;
    qreal lastAngle;
    QList<Provider> providers;
    bool connectionsVisible;
    Node() {

    }

    Node(QString name, qreal lat, qreal lng) : name(name), lat(lat), lng(lng)
    {
        lastAngle = 0;
        connectionsVisible = false;
    }
};

/**
 * @brief The NodeMapWidget class deals with drawing that is related to drawing Nodes, addresses, and connection on the map.
 */
class NodeMapWidget : public GMapWidget
{
    Q_OBJECT
public:
    explicit NodeMapWidget(QWidget *parent = 0);
    void addNodeMarker(QString nodeName, qreal lat, qreal lng);
    void addProviderMarker(QString nodeName, QString addressName);
    void addConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);
    void removeConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);

private:
    QHash<QString, Node> nodeMarkers;
    QHash<QString, Provider*> providerMarkers;
    QHash<QString, int> connectionSkews;
    QList<QString> selectedProviders;
    QMultiHash<QString, int> lineSkews;
    int getLineSkew(QString fromId, QString toId, int skewAmount);
    void removeLineSkew(QString fromId, QString toId, int skewAmount);
    int nextLineSkew(QString fromIp, QString toIp);
private slots:
    virtual void handleMapClicked();
    virtual void handleMarkerClicked(QString id, int category);
    virtual void handleMarkerHovered(QString id, int category);
    virtual void handleMarkerHoveredOff(QString id, int category);
signals:

    void providerSelected(QString nodeId, QString providerId);
    void providerHovered(QString nodeId, QString providerId);
    void providerHoveredOff(QString nodeId, QString providerId);

    void nodeSelected(QString nodeId);
    void nodeHovered(QString nodeId);
    void nodeHoveredOff(QString nodeId);

    void connectionRequest(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId);
};

#endif // NODEMAPWIDGET_H
