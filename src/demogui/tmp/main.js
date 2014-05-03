// main var
var map;
var markers = new Array();
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




// main init function
function initialize() {

    //mapWidget.helloSignal.connect(mapWidget.hello);
    var myOptions = {
        zoom: 12,
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    console.debug("successfully initialized");
    map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
    google.maps.event.addListener(map, 'click', function () {
        console.log("Firing");
        mapWidget.mapClicked();
    });
    google.maps.event.addListenerOnce(map,"projection_changed", function() {
      alert("projection:"+map.getProjection());
        mapWidget.mapLoaded();
    });

}

// custom functions
function setGMapCenter(lat, lng)
{
    map.setCenter(new google.maps.LatLng(lat, lng));
}

function setGMapZoom(zoom)
{
    map.setZoom(zoom);
}

function addMarker(name, lat, lng) {
    var latLng = new google.maps.LatLng(lat, lng);
    var marker = new google.maps.Marker({
                               position: latLng,
                               map: map,
                               title: name,
                                            icon: {
                                                url: 'marker.png',
                                                size: null,
                                                origin: null,
                                                anchor: new google.maps.Point(10,10),
                                                scaledSize: new google.maps.Size(20,20)

                           }});
    google.maps.event.addListener(marker, 'click', function () {
        mapWidget.markerSelected(marker.title);
    });

    google.maps.event.addListener(marker, 'mouseover', function () {
        mapWidget.markerHovered(marker.title);
    });
}

function addCustomMarker(title, id,  lat, lng, imageName, scaleX, scaleY, offsetX, offsetY) {
    var latLng = new google.maps.LatLng(lat, lng);
    var marker = new google.maps.Marker({
                               position: latLng,
                               map: map,
                               title: title,
                               id: id,
                                            icon: {
                                                url: imageName,
                                                size: null,
                                                origin: null,
                                                anchor: new google.maps.Point(offsetX,offsetY),
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
function showMarker(id) {
    console.log(id);
    markers[id].setVisible(true);
}

function hideMarker(id) {
    console.log(id);
    markers[id].setVisible(false);
}

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
