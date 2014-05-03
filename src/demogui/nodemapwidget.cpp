#include "nodemapwidget.h"
#include <QtMath>
#include <cmath>

NodeMapWidget::NodeMapWidget(QWidget *parent) : GMapWidget(parent)
{
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


void NodeMapWidget::addConnectionMarker(QString nodeName, QString addressName)
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
    QString id = addressName;
    Address address(addressName, nodeName, id, xOffset, yOffset);
    node.addresses << address;
    qDebug() <<"added address: " << id;
    node.lastAngle = angle;

    addressMarkers[id] = &node.addresses.last();

}

void NodeMapWidget::addConnectionLine(QString localIp, QString remoteIp, QString id)
{
    Address *localAddress = addressMarkers[localIp];
    Address *remoteAddress = addressMarkers[remoteIp];
    localAddress->connections++;
    remoteAddress->connections++;
    qDebug() <<"Increasing connection:" << localAddress->connections;
    Node &localNode = nodeMarkers[localAddress->nodeName];
    Node &remoteNode = nodeMarkers[remoteAddress->nodeName];
    int lineSkew = addConnection(localIp, remoteIp);
    qDebug() << "lineskew: " << lineSkew;
    qDebug() << "!!!ADDING!!!" << localAddress->id + "-" +  remoteAddress->id;
    drawLine(localIp + remoteIp + id, "tcp", localNode.lat, localNode.lng, remoteNode.lat, remoteNode.lng, -localAddress->offsetX, -localAddress->offsetY, -remoteAddress->offsetX, -remoteAddress->offsetY, lineSkew);
}

void NodeMapWidget::removeConnectionLine(QString fromIp, QString toIp)
{
    qDebug() << "Removing " << fromIp+toIp;
    removeLine(fromIp + toIp);
    Address *localAddress = addressMarkers[fromIp];
    Address *remoteAddress = addressMarkers[toIp];
    localAddress->connections--;
    remoteAddress->connections--;
}

int NodeMapWidget::addConnection(QString fromIp, QString toIp)
{
    QString id = fromIp + toIp;
    QString id2 = toIp + fromIp;
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
    for(int i = 0; i<300; i+=30) {
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

void NodeMapWidget::handleMapClicked()
{
    foreach(Node node, nodeMarkers) {
        if (node.connectionsVisible) {
            foreach(Address address, node.addresses) {
                //qDebug() << "!!!CONNECTION!!!" << address.connections;
                if (address.connections == 0) {
                    removeMarker(address.id);
                    removeLine(address.id);
                }
            }
            nodeMarkers[node.name].connectionsVisible = false;//te
        } else {
            qDebug() << "Connections already showing";
        }
    }

    foreach(QString id, selectedConnections) {
        Address *address = addressMarkers[id];
        changeIcon(id, "ipv4.png",20, 20, address->offsetX + 10, address->offsetY + 10);
    }
    selectedConnections.clear();

}


void NodeMapWidget::handleMarkerClicked(QString id, int category)
{
    if (category == 0) {
        Node &node = nodeMarkers[id];
        if (!node.connectionsVisible) {
            foreach(Address address, node.addresses) {
                if (address.connections > 0) continue;
                if (address.name.contains(".")) {
                    addCustomMarker(address.name, address.id, node.lat, node.lng, "ipv4.png",20, 20, address.offsetX + 10, address.offsetY + 10, 2);
                } else {
                    addCustomMarker(address.name, address.id,  node.lat, node.lng, "ipv6.png",20, 20, address.offsetX + 10, address.offsetY + 10, 2);
                }
                drawLine(address.id, "", node.lat, node.lng, node.lat, node.lng, 0, 0, -address.offsetX, -address.offsetY, 0, "black");
            }
            nodeMarkers[node.name].connectionsVisible = true;
        } else {
            qDebug() << "Connections already showing";
        }
        emit nodeSelected(id);
    } else {
        Address *address = addressMarkers[id];
        Node  &fromNode = nodeMarkers[address->nodeName];
        if (!selectedConnections.empty()) {
            Address *toAddress = addressMarkers[selectedConnections.last()];
            Node &toNode = nodeMarkers[toAddress->nodeName];
            QString toId = toAddress->id;
            if (address->name.contains(".")) {
                changeIcon(id, "ipv4.png",20, 20, address->offsetX + 10, address->offsetY + 10);
                changeIcon(toId, "ipv4.png",20, 20, toAddress->offsetX + 10, toAddress->offsetY + 10);
            } else {

                changeIcon(id, "ipv6.png",20, 20, address->offsetX + 10, address->offsetY + 10);
                changeIcon(toId, "ipv6.png",20, 20, toAddress->offsetX + 10, toAddress->offsetY + 10);
            }
            if (toAddress != address) {
                emit connectionRequest(fromNode.name, address->name, toAddress->name);
            }

            selectedConnections.clear();
        } else {
            selectedConnections << id;
            if (address->name.contains(".")) {
                changeIcon(id, "ipv4.png",26, 26, address->offsetX + 13, address->offsetY + 13);
            } else {
                changeIcon(id, "ipv6.png",26, 26, address->offsetX + 13, address->offsetY + 13);
            }
        }
        qDebug() << "Marker selected" << addressMarkers[id]->nodeName << addressMarkers[id]->name;
        //selectedConnections << id;
        emit addressSelected(addressMarkers[id]->nodeName, addressMarkers[id]->name);
    }

}

void NodeMapWidget::handleMarkerHovered(QString id, int category)
{
    qDebug() << "Hover";
    if (category == 0) {
        emit nodeHovered(id);
    } else {
        emit addressHovered(addressMarkers[id]->nodeName, addressMarkers[id]->name);
    }
}

void NodeMapWidget::handleMarkerHoveredOff(QString id, int category)
{
    qDebug() << "Hover off";
    if (category == 0) {
        emit nodeHoveredOff(id);
    } else {
        emit addressHoveredOff(addressMarkers[id]->nodeName, addressMarkers[id]->name);
    }
}
