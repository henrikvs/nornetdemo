#ifndef GRAPHDATA_H
#define GRAPHDATA_H
#include "qcustomplot.h"
#include "plotwindow.h"


struct BoxPlotData {
    qreal median;
    qreal firstQuartile;
    qreal thirdQuartile;
    qreal min;
    qreal max;
};

class GraphData
{
public:
    enum ExperimentType{TCP, PING};
    enum IPType{IPv4, IPv6};
    GraphData(ExperimentType expType, IPType ipType, QString name, QString srcAddr, QString destAddr, QString srcProviderId, QString destProviderId, QString srcNode,
              QString destNode, int sessionId);
    GraphData();
    QList<QCustomPlot*> plots;
    QList<QCPStatisticalBox*> statBoxes;
    QHash<PlotWindow*, QCPStatisticalBox*> plotToStatbox;
    QHash<PlotWindow*, QCPGraph*> plotToGraph;
    QCPDataMap data;
    QMultiMap<double, double> sortedData;


    void addData(qreal key, qreal value);
    void bindToWindow(PlotWindow *plotwin);
    void unbindWindow(PlotWindow *plotwin);

    ExperimentType expType;
    IPType ipType;

    QString expTypeString();
    QString ipTypeString();
    QString getSrcAddr();
    QString getDestAddr();
    QString getSrcNode();
    QString getDestNode();
    QString getName();
    QString getSrcProviderId();
    QString getDestProviderId();
    int  getsessionId();
private:
    BoxPlotData generateBoxPlotData();
    int type;
    qreal average;
    static int boxPos;
    QString name;
    QString srcAddr;
    QString destAddr;
    QString srcNode;
    QString destNode;
    QString srcProviderId;
    QString destProviderId;
    int sessionId;
};


#endif // GRAPHDATA_H
