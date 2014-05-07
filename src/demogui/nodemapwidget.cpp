#include "nodemapwidget.h"
#include <QtMath>
#include <cmath>

NodeMapWidget::NodeMapWidget(QWidget *parent) : GMapWidget(parent)
{
    qDebug() << "Msg here";
    connect(this, SIGNAL(mapClicked()), this, SLOT(handleMapClicked()));
    connect(this, SIGNAL(markerClicked(QString, int)), this, SLOT(handleMarkerClicked(QString, int)));
    connect(this, SIGNAL(markerHovered(QString,int)), this, SLOT(handleMarkerHovered(QString,int)));
    connect(this, SIGNAL(markerHoveredOff(QString,int)), this, SLOT(handleMarkerHoveredOff(QString,int)));
}

void NodeMapWidget::addNodeMarker(QString nodeName, qreal lat, qreal lng)
{
    nodeMarkers[nodeName] = Node(nodeName, lat, lng);
    //addCustomMarker("test", "nodeName" lat, lng, 0,0);
    addCustomMarker(nodeName, nodeName, lat, lng, "node.png", 30, 30, 15, 15);
    //addCustomMarker(nodeName, lat, lng, "ipv6.png", 25, 25, 0, 0);
    //addMarker(nodeName, lat, lng);
}


void NodeMapWidget::addProviderMarker(QString nodeName, QString providerId)
{
    if (!nodeMarkers.contains(nodeName)) {
        qDebug() << "addConnectionMarker: node name doesn't exist";
        return;
    }
    Node &node = nodeMarkers[nodeName];

    int angle = node.lastAngle;

    angle = angle+1.2;


    qreal xOffset = cos(angle) * 30;
    qreal yOffset = sin(angle) * 30;
    QString id =  providerId + "@" + nodeName;
    Provider provider(providerId, nodeName, id, xOffset, yOffset);
    node.providers << provider;
    qDebug() <<"added provider: " << id;
    node.lastAngle = angle;

    providerMarkers[id] = &node.providers.last();

}

void NodeMapWidget::addConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    Provider *localProvider = providerMarkers[srcId];
    Provider *remoteProvider = providerMarkers[destId];
    localProvider->connections++;
    remoteProvider->connections++;
    qDebug() <<"Increasing connection:" << localProvider->connections;
    Node &localNode = nodeMarkers[localProvider->nodeName];
    Node &remoteNode = nodeMarkers[remoteProvider->nodeName];
    QString idString = sessionId + ":" + srcId + ":" + destId;
    int lineSkew = getLineSkew(srcId, destId, 20);
    connectionSkews[idString] = lineSkew;
    qDebug() << "lineskew: " << lineSkew;
    qDebug() << "!!!ADDING!!!" << localProvider->id + "-" +  remoteProvider->id;
    drawLine(idString, "some info",  localNode.lat, localNode.lng, remoteNode.lat, remoteNode.lng, -localProvider->offsetX, -localProvider->offsetY, -remoteProvider->offsetX, -remoteProvider->offsetY, lineSkew);
}

void NodeMapWidget::removeConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    QString id = sessionId + ":" + srcId + ":" + destId;
    qDebug() << "Removing1 " << id;
    Provider *localProvider = providerMarkers[srcId];
    Provider *remoteProvider = providerMarkers[destId];
    localProvider->connections--;
    remoteProvider->connections--;
    if (localProvider->connections == 0) {
        removeMarker(localProvider->id);
        removeLine(localProvider->id);
    }

    if (remoteProvider->connections == 0) {
        removeMarker(remoteProvider->id);
        removeLine(remoteProvider->id);
    }
    removeLineSkew(srcId, destId, connectionSkews[id]);
    connectionSkews.remove(id);
    removeLine(id);
}

int NodeMapWidget::getLineSkew(QString fromId, QString toId, int skewAmount)
{
    QString id = fromId + toId;
    QString id2 = toId + fromId;
    bool oposite = false;
    QList<int> values;
    if (!lineSkews.values(id).empty()) {
        values = lineSkews.values(id);
    } else {
        oposite = true;
        values = lineSkews.values(id2);
        id = id2;
    }
    int matchValue = 0;
    bool match = true;
    for(int i = 0; i<300; i+=skewAmount) {
        matchValue = i;
        do {
            if (!values.contains(matchValue)) {
                lineSkews.insert(id, matchValue);
                if (oposite) {
                    matchValue = -matchValue;
                }
                return matchValue;
            }
            matchValue = -matchValue;
        }
        while (matchValue < 0);
    }
    return -1;
}

void NodeMapWidget::removeLineSkew(QString fromId, QString toId, int skewAmount)
{
    //qDebug() << "Attempting to remove lineskew" << fromId << toId << skewAmount;
    QString id = fromId + toId;
    QString id2 = toId + fromId;
    if (!lineSkews.values(id).empty()) {
        int ret = lineSkews.remove(id, skewAmount);
        qDebug() << "Removed lineskew" << ret << skewAmount;
    } else if (!lineSkews.values(id2).empty()) {
        int ret = lineSkews.remove(id2, skewAmount);
        qDebug() << "Removed lineskew" << ret << skewAmount;
    }
}

void NodeMapWidget::handleMapClicked()
{
    foreach(Node node, nodeMarkers) {
        if (node.connectionsVisible) {
            foreach(Provider provider, node.providers) {
                //qDebug() << "!!!CONNECTION!!!" << address.connections;
                if (provider.connections == 0) {
                    removeMarker(provider.id);
                    removeLine(provider.id);
                }
            }
            nodeMarkers[node.name].connectionsVisible = false;//te
        } else {
            qDebug() << "Connections already showing";
        }
    }

    foreach(QString id, selectedProviders) {
        Provider *provider = providerMarkers[id];
        changeIcon(id, "ipv4.png",20, 20, provider->offsetX + 10, provider->offsetY + 10);
    }
    selectedProviders.clear();

}


void NodeMapWidget::handleMarkerClicked(QString id, int category)
{
    if (category == 0) {
        Node &node = nodeMarkers[id];
        if (!node.connectionsVisible) {
            foreach(Provider provider, node.providers) {
                if (provider.connections > 0) continue;
                if (provider.name.contains(".")) {
                    addCustomMarker(provider.name, provider.id, node.lat, node.lng, "ipv4.png",20, 20, provider.offsetX + 10, provider.offsetY + 10, 2);
                } else {
                    addCustomMarker(provider.name, provider.id,  node.lat, node.lng, "ipv6.png",20, 20, provider.offsetX + 10, provider.offsetY + 10, 2);
                }
                drawLine(provider.id, "", node.lat, node.lng, node.lat, node.lng, 0, 0, -provider.offsetX, -provider.offsetY, 0, "black");
            }
            nodeMarkers[node.name].connectionsVisible = true;
        } else {
            qDebug() << "Connections already showing";
        }
        emit nodeSelected(id);
    } else {
        Provider *provider = providerMarkers[id];
        Node  &fromNode = nodeMarkers[provider->nodeName];
        if (!selectedProviders.empty()) {
            Provider *toProvider = providerMarkers[selectedProviders.last()];
            Node &toNode = nodeMarkers[toProvider->nodeName];
            QString toId = toProvider->id;
            if (provider->name.contains(".")) {
                changeIcon(id, "ipv4.png",20, 20, provider->offsetX + 10, provider->offsetY + 10);
                changeIcon(toId, "ipv4.png",20, 20, toProvider->offsetX + 10, toProvider->offsetY + 10);
            } else {

                changeIcon(id, "ipv6.png",20, 20, provider->offsetX + 10, provider->offsetY + 10);
                changeIcon(toId, "ipv6.png",20, 20, toProvider->offsetX + 10, toProvider->offsetY + 10);
            }
            if (toProvider != provider) {
                emit connectionRequest(toNode.name, toProvider->name, fromNode.name, provider->name);
            }

            selectedProviders.clear();
        } else {
            selectedProviders << id;
            if (provider->name.contains(".")) {
                changeIcon(id, "ipv4.png",26, 26, provider->offsetX + 13, provider->offsetY + 13);
            } else {
                changeIcon(id, "ipv6.png",26, 26, provider->offsetX + 13, provider->offsetY + 13);
            }
        }
        qDebug() << "Marker selected" << providerMarkers[id]->nodeName << providerMarkers[id]->name;
        //selectedConnections << id;
        emit providerSelected(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }

}

void NodeMapWidget::handleMarkerHovered(QString id, int category)
{
    qDebug() << "Hover";
    if (category == 0) {
        emit nodeHovered(id);
    } else {
        emit providerHovered(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }
}

void NodeMapWidget::handleMarkerHoveredOff(QString id, int category)
{
    qDebug() << "Hover off";
    if (category == 0) {
        emit nodeHoveredOff(id);
    } else {
        emit providerHoveredOff(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }
}
