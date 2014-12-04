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

MapWidget::MapWidget(QWidget *parent)
    : QWebView(parent)
{
    setPage(new MyPage());
    //QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    //QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);

    //diskCache->setCacheDirectory(QCoreApplication::applicationDirPath() + "/cache");
    //manager->setCache(diskCache);
    //page()->setNetworkAccessManager(manager);
    //page()->settings()->setMaximumPagesInCache(10);
    page()->mainFrame()->addToJavaScriptWindowObject("mapWidget", this); //register our interface



    //connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotNetworkReply(QNetworkReply*)));

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(handleLoadFinished(bool)));
}

void MapWidget::panToLocation(qreal lat, qreal lng)
{
    runScript(QString("panToLocation(%1, %2);").arg(lat).arg(lng));
}

void MapWidget::hideMarker(QString id)
{
    qDebug() << "Hiding marker";
    //runScript(QString("hideMarker('%1');").arg(id));
}

void MapWidget::showMarker(QString id)
{
    runScript(QString("showMarker('%1');").arg(id));
}

void MapWidget::removeMarker(QString id)
{
    runScript(QString("removeMarker('%1');").arg(id));
}

void MapWidget::drawLine(QString id, QString info, qreal fromLat, qreal fromLng, qreal toLat, qreal toLng, int xOffsetStart, int yOffsetStart, int xOffsetEnd, int yOffsetEnd, int lineSkew, QString color, bool interact)
{
   runScript(QString("drawLine(\"%9\",'%12', %1, %2, %3, %4, %5, %6, %7, %8, %10, '%11', %13);").arg(fromLat).arg(fromLng).arg(toLat).arg(toLng).arg(xOffsetStart).arg(yOffsetStart).arg(xOffsetEnd).arg(yOffsetEnd).arg(id).arg(lineSkew).arg(color).arg(info).arg(interact));
    //runScript(QString("drawLine(%1, %2, %3, %4, %5, %6, %7, %8);").arg(fromLat, fromLng, toLat, toLng, xOffsetStart, yOffsetStart, xOffsetEnd, yOffsetEnd);

}

void MapWidget::removeLine(QString id)
{
    runScript(QString("removeLine('%1');").arg(id));
}

void MapWidget::changeIcon(QString id, QString imageName, qreal scaleX, qreal scaleY, int offsetX, int offsetY)
{
    runScript(QString("changeIcon('%1', '%2', %3, %4, %5, %6);").arg(id).arg(imageName).arg(scaleX).arg(scaleY).arg(offsetX).arg(offsetY));
}


// wait till the load finished signal before initializing the map trough javascript functions!
// somethimes the geocoding is faster than the webview can load the google map runtime
void MapWidget::handleLoadFinished(bool ok)
{
    //page()->mainFrame()->evaluateJavaScript("console.log('Testing test');");
    //if(!m_initMap && ok) {
      //  m_initMap = true;

        // load a default location on map init
        //setGMapCenter("Antwerpen, Belgium");
        //qDebug("Set center");
    //}
}


void MapWidget::setLineColor(QString id, QString color)
{
    runScript(QString("setLineColor('%1', '%2');").arg(id).arg(color));
}

void MapWidget::addMarker(QString name, qreal lat, qreal lng)
{
    //runScript(QString("addMarker(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
}


void MapWidget::addMarker(QString name, qreal lat, qreal lng, int offsetX, int offsetY)
{
    qDebug() << "adding marker123: " << name << lat << lng;
    runScript(QString("addMarker(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
    //runScript(QString("addMarker2(\"%1\", %2, %3)").arg(name).arg(lat).arg(lng));
}


void MapWidget::addCustomMarker(QString name, QString id, qreal lat, qreal lng, QString imagepath, qreal scaleX, qreal scaleY, int offsetX, int offsetY, int rotation, int z, int category)
{
    runScript(QString("addCustomMarker(\"%1\", \"%2\", %3, %4, \"%5\", %6, %7, %8, %9, %11, %12, %10)").arg(name).arg(id).arg(lat).arg(lng).arg(imagepath).arg(scaleX).arg(scaleY).arg(offsetX).arg(offsetY).arg(category).arg(rotation).arg(z));
}

void MapWidget::addCustomSymbolMarker(QString name, qreal lat, qreal lng, QString symbolPath, qreal scale, int offsetX, int offsetY, int rotation)
{
    //runScript(QString("addCustomSymbolMarker(\"%1\", %2, %3, \"%4\", %5, %6, %7, %8)").arg(name).arg(lat).arg(lng).arg(symbolPath).arg(scale).arg(offsetX).arg(offsetY).arg(rotation));
}


void MapWidget::start()
{
    load(QUrl("qrc:/index.html"));
}

// run a javascript function
void MapWidget::runScript(const QString &script)
{
    page()->mainFrame()->evaluateJavaScript(script);
}
