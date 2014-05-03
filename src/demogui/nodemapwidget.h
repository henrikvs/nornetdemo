#ifndef NODEMAPWIDGET_H
#define NODEMAPWIDGET_H

#include "gmapwidget.h"
#include <QHash>


struct Address {
    qreal offsetX;
    qreal offsetY;
    QString name;
    QString nodeName;
    QString id;
    int connections;
    //QList<Address> connections;
    Address() {

    }
    Address(QString name, QString nodeName, QString id, qreal offsetX, qreal offsetY): name(name), nodeName(nodeName),
        id(id), offsetX(offsetX), offsetY(offsetY), connections(0) {

    }
};

struct Node {
    QString name;
    qreal lat;
    qreal lng;
    qreal lastAngle;
    QList<Address> addresses;
    bool connectionsVisible;
    Node() {

    }

    Node(QString name, qreal lat, qreal lng) : name(name), lat(lat), lng(lng)
    {
        lastAngle = 0;
        connectionsVisible = false;
    }
};

class NodeMapWidget : public GMapWidget
{
    Q_OBJECT
public:
    explicit NodeMapWidget(QWidget *parent = 0);
    void addNodeMarker(QString nodeName, qreal lat, qreal lng);
    void addConnectionMarker(QString nodeName, QString addressName);
    void addConnectionLine(QString localIp, QString remoteIp, QString id);
    void removeConnectionLine(QString fromIp, QString toIp);

private:
    QHash<QString, Node> nodeMarkers;
    QHash<QString, Address*> addressMarkers;
    QList<QString> selectedConnections;
    QMultiHash<QString, int> lineSkews;
    int addConnection(QString fromIp, QString toIp);
    int nextLineSkew(QString fromIp, QString toIp);
private slots:
    virtual void handleMapClicked();
    virtual void handleMarkerClicked(QString id, int category);
    virtual void handleMarkerHovered(QString id, int category);
    virtual void handleMarkerHoveredOff(QString id, int category);
signals:

    void addressSelected(QString nodeId, QString address);
    void addressHovered(QString nodeId, QString address);
    void addressHoveredOff(QString nodeId, QString address);

    void nodeSelected(QString nodeId);
    void nodeHovered(QString nodeId);
    void nodeHoveredOff(QString nodeId);

    void connectionRequest(QString nodeId, QString localIp, QString remoteIp);
};

#endif // NODEMAPWIDGET_H
