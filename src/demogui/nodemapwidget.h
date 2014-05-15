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

struct Connection {
    QString id;
    Provider *srcProvider;
    Provider *destProvider;
    Node *srcNode;
    Node *destNode;
    QString sessionId;
    int skew;

    Connection() {

    }
    Connection(Provider *srcProvider,Provider *destProvider,Node *srcNode,Node *destNode, QString sessionId):
        srcProvider(srcProvider), destProvider(destProvider), srcNode(srcNode), destNode(destNode), sessionId(sessionId){

    }
};

/**
 * @brief The NodeMapWidget class deals with drawing that is related to drawing Nodes, addresses, and connection on the map.
 */
class NodeMapWidget : public MapWidget
{
    Q_OBJECT
public:
    explicit NodeMapWidget(QWidget *parent = 0);
    void addNodeMarker(QString nodeName, qreal lat, qreal lng);
    void addProviderMarker(QString nodeName, QString addressName);
    void addConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);
    void removeConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);
    void drawConnectionTraffic(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);
    void removeNodeMarker(QString nodeName);
    void removeConnectionLine(QString connectionId);

private:
    QHash<QString, Node*> nodeMarkers;
    QHash<QString, Provider*> providerMarkers;
    QHash<QString, Connection> connections;
    QHash<QString, int> connectionSkews;
    QList<QString> selectedProviders;
    QMultiHash<QString, int> lineSkews;
    int getLineSkew(QString fromId, QString toId, int skewAmount);
    void removeLineSkew(QString fromId, QString toId, int skewAmount);
    int nextLineSkew(QString fromIp, QString toIp);
    void addConnectionLineOverlay(QString inst, QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);
    void removeConnectionLineOverlay(QString inst, QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId);

private slots:
    virtual void handleMapClicked();
    virtual void handleMarkerClicked(QString id, int category);
    virtual void handleMarkerHovered(QString id, int category);
    virtual void handleMarkerHoveredOff(QString id, int category);
    virtual void handleMarkerRightClicked(QString id, int category);
signals:

    void providerSelected(QString nodeId, QString providerId);
    void providerHovered(QString nodeId, QString providerId);
    void providerHoveredOff(QString nodeId, QString providerId);
    void providerRightClicked(QString nodeId, QString providerId);



    void nodeSelected(QString nodeId);
    void nodeHovered(QString nodeId);
    void nodeHoveredOff(QString nodeId);
    void nodeRightClicked(QString nodeId);

    void connectionRequest(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId);
};

#endif // NODEMAPWIDGET_H
