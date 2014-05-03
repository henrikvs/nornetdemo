#include "gmapwidget.h"
#include "mypage.h"

#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QWebFrame>
#include <QUrlQuery>
#include <QDebug>

GMapWidget::GMapWidget(QWidget *parent)
    : QWebView(parent)
    , m_accessManager(new QNetworkAccessManager(this))
    , m_initMap(false)
{
    setPage(new MyPage());
    //QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    //QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);

    //diskCache->setCacheDirectory(QCoreApplication::applicationDirPath() + "/cache");
    //manager->setCache(diskCache);
    //page()->setNetworkAccessManager(manager);
    //page()->settings()->setMaximumPagesInCache(10);
    page()->mainFrame()->addToJavaScriptWindowObject("mapWidget", this); //register our interface



    connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotNetworkReply(QNetworkReply*)));

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadFinished(bool)));
}

void GMapWidget::hideMarker(QString id)
{
    qDebug() << "Hiding marker";
    //runScript(QString("hideMarker('%1');").arg(id));
}

void GMapWidget::showMarker(QString id)
{
    runScript(QString("showMarker('%1');").arg(id));
}

void GMapWidget::removeMarker(QString id)
{
    runScript(QString("removeMarker('%1');").arg(id));
}

void GMapWidget::drawLine(QString id, QString info, qreal fromLat, qreal fromLng, qreal toLat, qreal toLng, int xOffsetStart, int yOffsetStart, int xOffsetEnd, int yOffsetEnd, int lineSkew, QString color)
{
   runScript(QString("drawLine(\"%9\",'%12', %1, %2, %3, %4, %5, %6, %7, %8, %10, '%11');").arg(fromLat).arg(fromLng).arg(toLat).arg(toLng).arg(xOffsetStart).arg(yOffsetStart).arg(xOffsetEnd).arg(yOffsetEnd).arg(id).arg(lineSkew).arg(color).arg(info));
    //runScript(QString("drawLine(%1, %2, %3, %4, %5, %6, %7, %8);").arg(fromLat, fromLng, toLat, toLng, xOffsetStart, yOffsetStart, xOffsetEnd, yOffsetEnd);

}

void GMapWidget::removeLine(QString id)
{
    runScript(QString("removeLine('%1');").arg(id));
}

void GMapWidget::changeIcon(QString id, QString imageName, qreal scaleX, qreal scaleY, int offsetX, int offsetY)
{
    runScript(QString("changeIcon('%1', '%2', %3, %4, %5, %6);").arg(id).arg(imageName).arg(scaleX).arg(scaleY).arg(offsetX).arg(offsetY));
}


// wait till the load finished signal before initializing the map trough javascript functions!
// somethimes the geocoding is faster than the webview can load the google map runtime
void GMapWidget::slotLoadFinished(bool ok)
{
    //page()->mainFrame()->evaluateJavaScript("console.log('Testing test');");
    //if(!m_initMap && ok) {
      //  m_initMap = true;

        // load a default location on map init
        //setGMapCenter("Antwerpen, Belgium");
        //qDebug("Set center");
    //}
}

// geocode location strings to Latitude / Longitude positions
void GMapWidget::setGMapCenter(const QString &location)
{
    /*
    QUrlQuery q;
    QUrl url("http://maps.googleapis.com/maps/api/geocode/xml");
    q.addQueryItem("address", location);
    q.addQueryItem("sensor", "false");
    url.setQuery(q);
    QNetworkRequest request(url);

    m_accessManager->get(request);
*/
}

// overload with QPointF
void GMapWidget::setGMapCenter(const QPointF &pos)
{
    setGMapCenter(pos.x(), pos.y());
}

// overload with qreal
void GMapWidget::setGMapCenter(qreal lat, qreal lng)
{
    //runScript(QString("setGMapCenter(%1,%2)").arg(lat).arg(lng));
    //addMarker("Test", lat, lng);
    //addMarker("Test2", 59.897573, 9.596022);
}

// set a zoom level on the map
void GMapWidget::setGMapZoom(int zoomLevel)
{
    //runScript(QString("setGMapZoom(%1)").arg(zoomLevel));
}

void GMapWidget::addMarker(QString name, qreal lat, qreal lng)
{
    //runScript(QString("addMarker(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
}


void GMapWidget::addMarker(QString name, qreal lat, qreal lng, int offsetX, int offsetY)
{
    qDebug() << "adding marker123: " << name << lat << lng;
    runScript(QString("addMarker(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
    //runScript(QString("addMarker2(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
}


void GMapWidget::addCustomMarker(QString name, QString id, qreal lat, qreal lng, QString imagepath, qreal scaleX, qreal scaleY, int offsetX, int offsetY, int category)
{
    runScript(QString("addCustomMarker(\"%1\", \"%2\", %3, %4, \"%5\", %6, %7, %8, %9, %10)").arg(name).arg(id).arg(lat).arg(lng).arg(imagepath).arg(scaleX).arg(scaleY).arg(offsetX).arg(offsetY).arg(category));
}

void GMapWidget::addCustomSymbolMarker(QString name, qreal lat, qreal lng, QString symbolPath, qreal scale, int offsetX, int offsetY, int rotation)
{
    //runScript(QString("addCustomSymbolMarker(\"%1\", %2, %3, \"%4\", %5, %6, %7, %8)").arg(name).arg(lat).arg(lng).arg(symbolPath).arg(scale).arg(offsetX).arg(offsetY).arg(rotation));
}


void GMapWidget::start()
{
    load(QUrl("qrc:/index.html"));
}

// read out a geocode xml reply from google
void GMapWidget::slotNetworkReply(QNetworkReply *reply)
{
    QXmlStreamReader reader(reply->readAll());

    while(!reader.atEnd()) {

        reader.readNext();

        if(reader.name() == "geometry") {

            reader.readNextStartElement();

            if(reader.name() == "location") {

                reader.readNextStartElement();

                if(reader.name() == "lat") {

                    QPointF pos;

                    pos.setX(reader.readElementText().toFloat());

                    reader.readNextStartElement();

                    if(reader.name() == "lng") {

                        pos.setY(reader.readElementText().toFloat());

                        setGMapCenter(pos);

                        return;
                    }
                }
            }
        }
    }
}

// run a javascript function
void GMapWidget::runScript(const QString &script)
{
    page()->mainFrame()->evaluateJavaScript(script);
}
