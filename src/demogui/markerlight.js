//by viswaug on December 18, 2008


function MarkerLight(latlng, opts, map) {
    console.log("MarkerLight");
    this.map_ = map;
    this.latlng = latlng;
    if (!opts) opts = {};
    this.height_ = opts.height || 32;
    this.width_ = opts.width || 32;
    this.image_ = opts.image;
    this.imageOver_ = opts.imageOver;
    this.clicked_ = 0;

    this.setMap(map);
    console.log("MarkerLight ended");
}

MarkerLight.prototype = new google.maps.OverlayView();

MarkerLight.prototype.onAdd = function() {
    console.log("on add");
    var me = this;

    // Create the DIV representing our MarkerLight
    var div = document.createElement("div");
    div.style.borderStyle = "none";
    div.style.borderWidth = '0px';
    div.style.position = "absolute";
    div.style.paddingLeft = "0px";
    //div.style.cursor = 'pointer';

    var img = document.createElement("img");
    console.log(me.image_);
    img.src = me.image_;
    img.style.width = me.width_ + "px";
    img.style.height = me.height_ + "px";
    console.log(img.style.width);
    console.log(img.style.height);
    div.appendChild(img);

    /*GEvent.addDomListener(div, "click", function(event) {
        me.clicked_ = 1;
        GEvent.trigger(me, "click");
    });*/

    this.div_ = div;
    var panes = this.getPanes();
    panes.overlayImage.appendChild(div);
};


/* MarkerLight extends GOverlay class from the Google Maps API
*/

/* Creates the DIV representing this MarkerLight.
* @param map {GMap2} Map that bar overlay is added to.
*/

    /* Remove the main DIV from the map pane
     */
MarkerLight.prototype.remove = function() {
    this.div_.parentNode.removeChild(this.div_);
};

/* Copy our data to a new MarkerLight
* @return {MarkerLight} Copy of bar
*/
MarkerLight.prototype.copy = function() {
    var opts = {};
    opts.color = this.color_;
    opts.height = this.height_;
    opts.width = this.width_;
    opts.image = this.image_;
    opts.imageOver = this.image_;
    return new MarkerLight(this.latlng, opts);
};
/* Redraw the MarkerLight based on the current projection and zoom level
* @param force {boolean} Helps decide whether to redraw overlay
*/

MarkerLight.prototype.draw = function() {
    console.log("draw started)");
    var overlayProjection = this.getProjection();
    // Calculate the DIV coordinates of two opposite corners
    // of our bounds to get the size and position of our MarkerLight
    console.log(this.latlng.lat() + " : " + this.latlng.lng());
    var divPixel = overlayProjection.fromLatLngToContainerPixel(this.latlng);
        // Now position our DIV based on the DIV coordinates of our bounds

    //if ((divPixel.x > -500) && (divPixel.y > -500)) {
        this.div_.style.width = this.width_ + "px";
        this.div_.style.left = (divPixel.x) + "px";
        this.div_.style.height = (this.height_) + "px";
        this.div_.style.top = divPixel.y + "px";
        this.div_.style.display = "block";
    //}
        console.log("width: " + this.div_.style.width + "height:" + this.div_.style.height + "left:" + this.div_.style.left + " top:" + this.div_.style.top);
};

MarkerLight.prototype.redraw = function () {
    console.log("Redrawing");
}

MarkerLight.prototype.getZIndex = function(m) {
    return GOverlay.getZIndex(marker.getPoint().lat())-m.clicked*10000;
}

MarkerLight.prototype.getPoint = function() {
    return this.latlng;
};

MarkerLight.prototype.setStyle = function(style) {
    for (s in style) {
        this.div_.style[s] = style[s];
    }
};
MarkerLight.prototype.setImage = function(image) {
    this.div_.style.background = 'url("' + image + '")';
}
