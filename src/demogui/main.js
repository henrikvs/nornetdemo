// main var
var map;
var markers = {};
var lines = {};

/*
function onClick(event) {
    console.dir(event);
    mapWidget.markerSelected(event.target.name);
}

function onHover(event) {
    console.info(event);
    mapWidget.markerHovered(event.target.name);
}

function offHover(event) {
    console.info(event);
    mapWidget.markerHoveredOff(event.target.name);
}


*/


L.Polyline2 = L.Polyline.extend({
                                    options: {
                                        // how much to simplify the polyline on each zoom level
                                        // more = better performance and smoother look, less = more accurate
                                        xOffsetStart: 0,
                                        yOffsetStart: 0,
                                        xOffsetEnd: 0,
                                        yOffsetEnd: 0,
                                        skewValue: 0
                                    },
                                    projectLatlngs: function () {
                                        console.log("Projectlatlngs");
                                        this._originalPoints = [];
                                        var points = [];

                                        for (var i = 0, len = this._latlngs.length; i < len; i++) {
                                            var point = this._map.latLngToLayerPoint(this._latlngs[i]);
                                            if (i == 0) {
                                                point = L.point(point.x + this.options.xOffsetStart, point.y + this.options.yOffsetStart);
                                            } else if (i == (len-1)) {
                                                point = L.point(point.x + this.options.xOffsetEnd, point.y + this.options.yOffsetEnd);
                                            }

                                            this._originalPoints[i] = point;
                                            points[i] = point;
                                        }
                                        var vector = points[1].subtract(points[0]);

                                        var middlePoint = vector.divideBy(2.0).add(points[0]);

                                        var length = vector.distanceTo(L.point(0.0,0.0));


                                        var normVect = vector.divideBy(length);

                                        var rotatednormVect = L.point(normVect.y, -normVect.x);
                                        var rotatedVect = rotatednormVect.multiplyBy(this.options.skewValue);
                                        var skewedPoint = middlePoint.add(rotatedVect);
                                        console.log(skewedPoint.x + "."  + skewedPoint.y);
                                        this._originalPoints = [points[0], skewedPoint, points[1]];
                                    },
                                    _animateZoom: function (opt) {
                                        console.log("Animate zoooooooom");
                                        //var pos = this._map._latLngToNewLayerPoint(this._latlng, opt.zoom, opt.center).round();

                                        //this._setPos(pos);
                                    }

                                });

L.polyline2 = function (latlngs, options) {
    return new L.Polyline2(latlngs, options);
};

//L.Polyline2.addInitHook(function () {
//    this._zoomAnimated = true;
//    console.log("Do something here");
//});

// main init function
function initialize() {

    //mapWidget.helloSignal.connect(mapWidget.hello);
   /* var myOptions = {
        zoom: 12,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    */console.debug("successfully initialized");
    map = L.map('map_canvas',
                {
                    //center: L.latLng(60.285822, 10.369141)
                });
    console.log("loading 2");

    map.on('load', function() {
        console.log("loaded");
            map.setView(L.latLng(60.285822, 10.369141));
            map.setZoom(5);
        mapWidget.mapLoaded();
    });

    map.on('viewreset', function() {
        console.log("view resettet");
    });

    map.on('zoomanim', function() {
        console.log("Animating zoom");
    });

    map.on('click', function() {
        console.log("Map clicked");
       mapWidget.mapClicked();
    });

    map.on('mouseover', function() {
        console.log("Map clicked");
       mapWidget.mapHovered();
    });

    map.on('contextmenu', function() {
        console.log("Map clicked");
       mapWidget.mapRightClicked();
    });

    map.setView([51.505, -0.09], 13);

    L.tileLayer('http://{s}.tile.openstreetmap.fr/hot/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors',
        //attributionControl: false,
        updateWhenIdle: false,
        unloadInvisibleTiles: false,
    }).addTo(map);


    /*google.maps.event.addListenerOnce(map,"projection_changed", function() {
      alert("projection:"+map.getProjection());
        mapWidget.mapLoaded();
    });*/

}

// custom functions
/*
function setGMapCenter(lat, lng)
{
    map.setCenter(new google.maps.LatLng(lat, lng));
}

function setGMapZoom(zoom)
{
    map.setZoom(zoom);
}
*/

function drawLine(id, info, fromLat, fromLng, toLat, toLng, xOffsetStart, yOffsetStart, xOffsetEnd, yOffsetEnd, lineSkew, color, interact) {
    var clickable;
    if (interact) {
        clickable = true;
    } else {
        clickable = false;
    }

    var polyline = L.polyline2([L.latLng(fromLat, fromLng), L.latLng(toLat,toLng)],
                               {xOffsetStart: xOffsetStart,
                                yOffsetStart: yOffsetStart,
                                xOffsetEnd: xOffsetEnd,
                                yOffsetEnd: yOffsetEnd,
                                clickable: clickable,
                                skewValue: lineSkew,
                                weight: 10,
                                opacity: 0.5,
                                color: color});
    polyline.addTo(map);
    lines[id] = polyline;
    console.log("Adding id: " + id);

    if (interact) {
        var popup;
        polyline.on('click', function () {
            //alert("You clicked the map");
            console.log("Clicked");
            mapWidget.connectionClicked(id);
        });

        polyline.on('mouseover', function () {
            //alert("You clicked the map");
            console.log("Somethign");
            mapWidget.connectionHovered(id);
        });

        polyline.on('mouseout', function () {
            //alert("You clicked the map");
            console.log("Somethign2");
            mapWidget.connectionHoveredOff(id);
        });

        polyline.on('contextmenu', function() {
            mapWidget.connectionRightClicked(id);
            //console.log("Polyline rightclick");
        });
    //map.on('zoomanim', this.update());
    }

    /*polyline.on('mouseover', function(evt) {
        console.log("mouseon");
        polyline.bindPopup(info).openPopup();
        popup = L.popup({closeButton: false, autoPan: false}).setLatLng(evt.latlng).setContent(info);
        map.openPopup(popup);
    });

    polyline.on('mouseout', function(evt) {
        console.log("mouseoff");
        //polyline.bindPopup(info).openPopup();
        //map.closePopup(popup);
    });*/

}

function setLineColor(id, color) {
    line = lines[id];
    console.log("Options set");
    L.setOptions(line, {color: color});
    mapWidget.doRefresh();
}

function addMarker(name, lat, lng) {
    console.log("adding marker");
    L.marker([lat, lng]).addTo(map)
        .bindPopup('A pretty CSS3 popup. <br> Easily customizable.')
        .openPopup();
}

function changeIcon(id, imageName, scaleX, scaleY, offsetX, offsetY) {
    var icon = L.icon({
        iconUrl: imageName,
        //shadowUrl: 'leaf-shadow.png',

        iconSize:     [scaleX, scaleY], // size of the icon
        //shadowSize:   [50, 64], // size of the shadow
        iconAnchor:   [offsetX, offsetY], // point of the icon which will correspond to marker's location
        //shadowAnchor: [4, 62],  // the same for the shadow
        popupAnchor:  [offsetX, offsetY] // point from which the popup should open relative to the iconAnchor
    });

    markers[id].setIcon(icon);
}

function addCustomMarker(title, id,  lat, lng, imageName, scaleX, scaleY, offsetX, offsetY, rotation, z, category) {
    category = category || 0;
    var icon = L.icon({
        iconUrl: imageName,
        //shadowUrl: 'leaf-shadow.png',

        iconSize:     [scaleX, scaleY], // size of the icon
        shadowSize:   [50, 64], // size of the shadow
        iconAnchor:   [offsetX, offsetY], // point of the icon which will correspond to marker's location
        //shadowAnchor: [4, 62],  // the same for the shadow
        popupAnchor:  [offsetX, offsetY] // point from which the popup should open relative to the iconAnchor
    });
    marker = L.rotatedMarker([lat,lng], {icon: icon, angle: rotation, zIndexOffset: z});
    marker.addTo(map);
    marker.on('click', function () {
        mapWidget.markerClicked(id, category);
        console.log("Marker selected: " + id + ":" + category);
    });
    marker.on('mouseover', function() {
        mapWidget.markerHovered(id, category);
    });
    marker.on('mouseout', function() {
        mapWidget.markerHoveredOff(id, category);
    });
    marker.on('contextmenu', function() {
       console.log("Marker Right clic!!");
    });

    marker.on('dragend', function(event){
        //var marker = event.target;
        //var position = marker.getLatLng();
        alert(position);
        //marker.setLatLng([position],{id:uni,draggable:'true'}).bindPopup(position).update();
    });

    markers[id] = marker;
    //polyline._projectLatlngs();
    //polyline.setOffset(50,50);

    //map.on('zoomanim', this.update());
    console.log("Adding marker: " + id);
}
/*
function addCustomMarker3(title, id,  lat, lng, imageName, scaleX, scaleY, offsetX, offsetY) {
    var latLng = new google.maps.LatLng(lat, lng);
    var marker = new MarkerLight(latLng, {image: imageName, height:25, width:25}, map);

    markers[id] = marker;
}

function addCustomMarker2(title, id,  lat, lng, imageName, scaleX, scaleY, offsetX, offsetY) {
    var latLng = new google.maps.LatLng(lat, lng);
    console.log(map);
    var point1 = map.getProjection().fromLatLngToPoint(latLng);
    var latLng2 = map.getProjection().fromPointToLatLng(new google.maps.Point(point1.x + offsetX, point1.y + offsetY));
    var marker = new google.maps.Marker({
                               position: latLng2,
                               map: map,
                               title: title,
                               id: id,
                                            icon: {
                                                url: imageName,
                                                size: null,
                                                origin: null,
                                                //anchor: new google.maps.Point(offsetX,offsetY),
                                                scaledSize: new google.maps.Size(scaleX,scaleY)

                           }});
    google.maps.event.addListener(marker, 'click', function () {
        console.log("Firing");
        mapWidget.markerSelected(marker.id);
    });

    google.maps.event.addListener(marker, 'mouseover', function () {
        mapWidget.markerHovered(marker.id);
    });

    markers[id] = marker;
}
*/
function showMarker(id) {
    console.log(id);
    map.addLayer(markers[id]);
    //markers[id].setVisible(true);
}

function removeMarker(id) {
    console.log("Removing: "  + id);
    map.removeLayer(markers[id]);
    mapWidget.doRefresh();
    //map.addLayer(markers[id]);
    //markers[id].setVisible(true);
}

function removeLine(id) {
    console.log("Removing: "  + id);
    map.removeLayer(lines[id]);
    mapWidget.doRefresh();
}

function hideMarker(id) {
    console.log(id);
    map.removeLayer(markers[id]);
    //markers[id].setVisible(false);
}
/*
function addCustomSymbolMarker(name, lat, lng, symbolName, scale, offsetX, offsetY, rotation) {
    var latLng = new google.maps.LatLng(lat, lng);
    var marker = new google.maps.Marker({
                               position: latLng,
                               map: map,
                               title: name,
                                            icon: {
                                                path: "M69.224,59.474c0,0.205-0.064,0.41-0.195,0.583c-0.262,0.348-0.725,0.478-1.13,0.317L55.813,55.6v27.981 \
    c0,0.535-0.434,0.969-0.97,0.969h-9.686c-0.534,0-0.968-0.434-0.968-0.969V55.6l-12.088,4.774c-0.404,0.16-0.867,0.03-1.129-0.317 \
    c-0.263-0.348-0.262-0.828,0.006-1.174L49.232,35.14c0.184-0.239,0.467-0.378,0.768-0.378s0.585,0.139,0.768,0.378l18.255,23.743 \
    C69.156,59.057,69.224,59.266,69.224,59.474z M50,15.45c-4.524,0-8.192,3.668-8.192,8.191c0,4.525,3.668,8.193,8.192,8.193 \
    s8.193-3.668,8.193-8.193C58.193,19.118,54.524,15.45,50,15.45z",
                                                scale: scale,
                                                origin: null,
                                                rotation: rotation,
                                                anchor: new google.maps.Point(offsetX,offsetY)

                           }});
    google.maps.event.addListener(marker, 'click', function () {
        mapWidget.markerSelected(marker.title);
    });

    google.maps.event.addListener(marker, 'mouseover', function () {
        mapWidget.markerHovered(marker.title);
    });
}

function addMarker2(name, lat, lng) {
    var latLng = new google.maps.LatLng(lat, lng);
    var marker = new google.maps.Marker({
                               position: latLng,
                               map: map,
                               title: name,
                                            icon: {
                                                path: google.maps.SymbolPath.CIRCLE,
                                                scale: 5,
                                                strokeColor: 'blue',
                                                anchor: new google.maps.Point(2,2)

                           }});
    google.maps.event.addListener(marker, 'click', function () {
        mapWidget.markerSelected(marker.title);
    });

    google.maps.event.addListener(marker, 'mouseover', function () {
        mapWidget.markerHovered(marker.title);
    });

    google.maps.event.addListener(marker, 'mouseout', function () {
        mapWidget.markerHoveredOff(marker.title);
    });


}
*/
