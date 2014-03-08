import QtQuick 2.0
import QtLocation 5.0
import QtPositioning 5.2

Map {

    id: map
    plugin : Plugin {name : "osm"}
    anchors.fill: parent
    center: QtPositioning.coordinate(-27.5, 153.1)
}
