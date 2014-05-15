#ifndef GMAPWIDGET_H
#define GMAPWIDGET_H

#include <QWebView>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class MapWidget : public QWebView
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);

    void hideMarker(QString id);
    void showMarker(QString id);
    void removeMarker(QString id);
    void drawLine(QString id, QString info, qreal fromLat,qreal fromLng,qreal toLat,qreal toLng,int xOffsetStart,int yOffsetStart,int xOffsetEnd,int yOffsetEnd, int lineSkew, QString color = "red", bool interact = true);
    void removeLine(QString id);
    void changeIcon(QString id, QString imageName, qreal scaleX, qreal scaleY, int offsetX,int offsetY);
    void setLineColor(QString id, QString color);

public slots:
    void setGMapCenter(const QString &location);
    void setGMapCenter(const QPointF &pos);
    void setGMapCenter(qreal lat, qreal lng);

    void setGMapZoom(int zoomLevel);


    void addMarker(QString name, qreal lat, qreal lng);
    void addMarker(QString name, qreal lat, qreal lng, int offsetX, int offsetY);
    void addCustomMarker(QString name, QString id, qreal lat, qreal lng, QString imagepath, qreal scaleX, qreal scaleY, int offsetX, int offsetY, int category = 0);
    void addCustomSymbolMarker(QString name, qreal lat, qreal lng, QString symbolPath,qreal scale,int offsetX,int offsetY,int rotation);
    void start();

private slots:
    void handleLoadFinished(bool ok);
    virtual void handleMapClicked() {};
    virtual void handleMarkerClicked(QString, int) {};

protected:
    void runScript(const QString &script);

private:
signals:
    void markerClicked(QString id, int category);
    void markerHovered(QString name, int category);
    void markerHoveredOff(QString name, int category);
    void markerRightClicked(QString id, int category);
    void mapLoaded();
    void mapClicked();
    void mapHovered();
    void mapRightClicked();
    void doRefresh();

    void connectionClicked(QString id);
    void connectionRightClicked(QString id);
    void connectionHovered(QString id);
    void connectionHoveredOff(QString id);
};

#endif // GMAPWIDGET_H
