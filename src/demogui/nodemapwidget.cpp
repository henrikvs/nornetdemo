#include "nodemapwidget.h"
#include <QtMath>
#include <QTimer>
#include <cmath>

NodeMapWidget::NodeMapWidget(QWidget *parent) : MapWidget(parent)
{
    qDebug() << "Msg here";
    connect(this, SIGNAL(mapClicked()), this, SLOT(handleMapClicked()));
    connect(this, SIGNAL(markerClicked(QString, int)), this, SLOT(handleMarkerClicked(QString, int)));
    connect(this, SIGNAL(markerHovered(QString,int)), this, SLOT(handleMarkerHovered(QString,int)));
    connect(this, SIGNAL(markerHoveredOff(QString,int)), this, SLOT(handleMarkerHoveredOff(QString,int)));
    connect(this, SIGNAL(markerRightClicked(QString,int)), this, SLOT(handleMarkerRightClicked(QString,int)));
}

/**
 * @brief Adds a node marker
 * @param Name of the node, should be a unique name
 * @param lat
 * @param lng
 */
void NodeMapWidget::addNodeMarker(QString nodeName, qreal lat, qreal lng)
{
    nodeMarkers[nodeName] = Node(nodeName, lat, lng);
    //addCustomMarker("test", "nodeName" lat, lng, 0,0);
    addCustomMarker(nodeName, nodeName, lat, lng, "node.png", 30, 30, 15, 15);
    //addCustomMarker(nodeName, lat, lng, "ipv6.png", 25, 25, 0, 0);
    //addMarker(nodeName, lat, lng);
}



/**
 * @brief Adds a provider marker, connected to a node
 * @param The name of the node
 * @param The provider id. This is a globally unique id for which provider is used. It's not unique for this node.
 */
void NodeMapWidget::addProviderMarker(QString nodeName, QString providerId)
{
    if (!nodeMarkers.contains(nodeName)) {
        qDebug() << "addConnectionMarker: node name doesn't exist";
        return;
    }
    Node &node = nodeMarkers[nodeName];

    int angle = node.lastAngle;

    angle = angle+2;


    qreal xOffset = cos(angle) * 22;
    qreal yOffset = sin(angle) * 22;
    QString id =  providerId + "@" + nodeName;
    Provider provider(providerId, nodeName, id, xOffset, yOffset);
    node.providers << provider;
    qDebug() <<"added provider: " << id;
    node.lastAngle = angle;

    providerMarkers[id] = &node.providers.last();

}

/**
 * @brief Used to add a line between two providers
 * @param srcNodeId
 * @param srcProviderId
 * @param destNodeId
 * @param destProviderId
 * @param The session id of the connection that will be established.
 */
void NodeMapWidget::addConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    Provider *localProvider = providerMarkers[srcId];
    Provider *remoteProvider = providerMarkers[destId];
    localProvider->connections++;
    remoteProvider->connections++;
    //qDebug() <<"Increasing connection:" << localProvider->connections;
    Node &localNode = nodeMarkers[localProvider->nodeName];
    Node &remoteNode = nodeMarkers[remoteProvider->nodeName];
    QString idString = sessionId + ":" + srcId + ":" + destId;

    qDebug() << "Generating skew:" << srcId << destId;
    int lineSkew = getLineSkew(srcId, destId, 20);
    connectionSkews[idString] = lineSkew;
    connections[idString] = Connection(localProvider, remoteProvider, &localNode, &remoteNode, sessionId);
    //qDebug() << "lineskew: " << lineSkew;
    //qDebug() << "!!!ADDING!!!" << localProvider->id + "-" +  remoteProvider->id;
    drawLine(idString, "some info",  localNode.lat, localNode.lng, remoteNode.lat, remoteNode.lng, -localProvider->offsetX, -localProvider->offsetY, -remoteProvider->offsetX, -remoteProvider->offsetY, lineSkew, "blue");
}

/**
 * @brief Removed a connection line between two providers
 * @param srcNodeId
 * @param srcProviderId
 * @param destNodeId
 * @param destProviderId
 * @param The session id of the connection that will be established.
 */
void NodeMapWidget::removeConnectionLine(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    QString id = sessionId + ":" + srcId + ":" + destId;
    removeConnectionLine(id);
}

void NodeMapWidget::removeConnectionLine(QString connectionId)
{
    if (!connections.contains(connectionId)) {
        return;
    }
    Connection connection = connections.take(connectionId);
    qDebug() << "Removing1 " << connectionId;
    connection.srcProvider->connections--;
    connection.destProvider->connections--;
    if (connection.srcProvider->connections == 0 && !connection.srcNode->connectionsVisible) {
        removeMarker(connection.srcProvider->id);
        //removeLine(localProvider->id);
    }

    if (connection.destProvider->connections == 0 && !connection.destNode->connectionsVisible) {
        removeMarker(connection.destProvider->id);
        //removeLine(remoteProvider->id);
    }
    QString srcId = connection.srcProvider->name + "@" + connection.srcProvider->nodeName;
    QString destId = connection.destProvider->name + "@" + connection.destProvider->nodeName;
    qDebug() << "Removing skew: " << srcId << destId;
    removeLineSkew(srcId, destId, connectionSkews[connectionId]);
    connectionSkews.remove(connectionId);
    removeLine(connectionId);
}

void NodeMapWidget::drawConnectionTraffic(QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
/*
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    QString id = sessionId + ":" + srcId + ":" + destId;

    setLineColor(id, "blue");*/

    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    QString idString = sessionId + ":" + srcId + ":" + destId;
    if (!connections.contains(idString)) {
        qDebug() << "connection doesn't exist";
        return;
    }
    static int next = 0;
    next++;
    int inst = next;
    addConnectionLineOverlay(QString::number(next), srcNodeId, srcProviderId, destNodeId, destProviderId, sessionId);
    qDebug() << "Adding connection line session id: " << sessionId;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this, timer, srcNodeId, srcProviderId, destNodeId, destProviderId, sessionId, inst]() {
        //qDebug() << "Removed";
        removeConnectionLineOverlay(QString::number(inst), srcNodeId, srcProviderId, destNodeId, destProviderId, sessionId);
        timer->stop();
        //timer->deleteLater();
    });
    timer->start(300);
}

void NodeMapWidget::removeNodeMarker(QString nodeName)
{
    removeMarker(nodeName);
    Node &node = nodeMarkers[nodeName];
    foreach(Provider provider, node.providers) {
        removeMarker(provider.id);
    }


}

/**
 * @brief Used  to avoid overlap of lines when adding lines between providers that already has a line
 * @param fromId
 * @param toId
 * @param The amount of skew between each line
 * @return The resulting skew to this line.
 */
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

/**
 * @brief Frees the line path for future lines
 * @param fromId
 * @param toId
 * @param The actual amount of skew that this line had
 */
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

void NodeMapWidget::addConnectionLineOverlay(QString inst, QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    Provider *localProvider = providerMarkers[srcId];
    Provider *remoteProvider = providerMarkers[destId];
    qDebug() <<"Increasing connection:" << localProvider->connections;
    Node &localNode = nodeMarkers[localProvider->nodeName];
    Node &remoteNode = nodeMarkers[remoteProvider->nodeName];
    QString idString = sessionId + ":" + srcId + ":" + destId;
    int lineSkew = connectionSkews[idString];
    qDebug() << "lineskew: " << lineSkew;
    qDebug() << "!!!ADDING!!!" << localProvider->id + "-" +  remoteProvider->id;
    drawLine(inst + idString, "some info",  localNode.lat, localNode.lng, remoteNode.lat, remoteNode.lng, -localProvider->offsetX, -localProvider->offsetY, -remoteProvider->offsetX, -remoteProvider->offsetY, lineSkew, "blue", false);
}

void NodeMapWidget::removeConnectionLineOverlay(QString inst, QString srcNodeId, QString srcProviderId, QString destNodeId, QString destProviderId, QString sessionId)
{
    QString srcId = srcProviderId + "@" + srcNodeId;
    QString destId = destProviderId + "@" + destNodeId;
    QString id = sessionId + ":" + srcId + ":" + destId;
    removeLine(inst + id);
}

/**
 * @brief Handles what to do when just the map is clicked (No markers, providers or connections)
 * It will hide all providers that are not in use.
 */
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
        changeIcon(id, "marker3.png",20, 20, provider->offsetX + 10, provider->offsetY + 10);
    }
    selectedProviders.clear();

}

/**
 * @brief Handles what to do when a marker is clicked
 * A marker is either a node marker, or a provider marker. If it's a nodemarker:
 * - It will draw all available providers for this marker on the map
 * If it's a provider marker:
 * - If a marker was clicked previously, it will attempt to establish an experiment connection.
 * - If not, it will add the marker to a list of selected markers, and also change the icon to represent that the node is clicked
 * @param id
 * @param category
 */
void NodeMapWidget::handleMarkerClicked(QString id, int category)
{
    if (category == 0) {
        Node &node = nodeMarkers[id];
        if (!node.connectionsVisible) {
            foreach(Provider provider, node.providers) {
                if (provider.connections > 0) continue;
                addCustomMarker(provider.name, provider.id,  node.lat, node.lng, "marker3.png",20, 20, provider.offsetX + 10, provider.offsetY + 10, 2);
                //drawLine(provider.id, "", node.lat, node.lng, node.lat, node.lng, 0, 0, -provider.offsetX, -provider.offsetY, 0, "black");
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
            changeIcon(id, "marker3.png",20, 20, provider->offsetX + 10, provider->offsetY + 10);
            changeIcon(toId, "marker3.png",20, 20, toProvider->offsetX + 10, toProvider->offsetY + 10);
            if (toProvider != provider) {
                emit connectionRequest(toNode.name, toProvider->name, fromNode.name, provider->name);
            }

            selectedProviders.clear();
        } else {
            selectedProviders << id;
            changeIcon(id, "marker3.png",26, 26, provider->offsetX + 13, provider->offsetY + 13);
        }
        qDebug() << "Marker selected" << providerMarkers[id]->nodeName << providerMarkers[id]->name;
        //selectedConnections << id;
        emit providerSelected(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }

}

/**
 * @brief Handles what to do if a marker is hovered
 * Emits a signal to allow connected object to generate a response to this (For example show information in a popup)
 * @param id
 * @param category
 */
void NodeMapWidget::handleMarkerHovered(QString id, int category)
{
    qDebug() << "Hover";
    if (category == 0) {
        emit nodeHovered(id);
    } else {
        emit providerHovered(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }
}

/**
 * @brief Handles what to do if a marker is no longer hovered
 * @param Emits a signal to allow connected object to generate a response to this (For example close a popup)
 * @param category
 */
void NodeMapWidget::handleMarkerHoveredOff(QString id, int category)
{
    qDebug() << "Hover off";
    if (category == 0) {
        emit nodeHoveredOff(id);
    } else {
        emit providerHoveredOff(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }
}

void NodeMapWidget::handleMarkerRightClicked(QString id, int category)
{
    if (category == 0) {
        emit nodeRightClicked(id);
    } else {
        emit providerRightClicked(providerMarkers[id]->nodeName, providerMarkers[id]->name);
    }
}
