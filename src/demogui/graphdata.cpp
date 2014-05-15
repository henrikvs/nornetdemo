#include "graphdata.h"


GraphData::GraphData(GraphData::ExperimentType expType, GraphData::IPType ipType, QString name, QString srcAddr, QString destAddr,
                     QString srcNode, QString destNode, QString srcProviderId, QString destProviderId, int sessionId):
    expType(expType), ipType(ipType), name(name), srcAddr(srcAddr), destAddr(destAddr),
    srcNode(srcNode), destNode(destNode), srcProviderId(srcProviderId), destProviderId(destProviderId),sessionId(sessionId)
{
    this->name = expTypeString() + " - " + ipTypeString();
}

GraphData::GraphData()
{
}

bool operator< (QCPData d1, QCPData d2)
{
    return (d1.value < d2.value);
}

bool operator> (QCPData d1, QCPData d2)
{
    return (d1.value > d2.value);
}


void GraphData::addData(qreal key, qreal value)
{
    average = (average * data.size()) / (data.size() + 1);
    data[key] = QCPData(key, value);
    sortedData.insertMulti(value, value);

    foreach(QCustomPlot *plot, plots) {
        qDebug() << "Replotting" << plots.size();
        plot->rescaleAxes(false);
        plot->replot();
    }


    BoxPlotData boxData = generateBoxPlotData();


    foreach(QCPStatisticalBox *box, statBoxes) {
        qDebug() << "Replotting statboxes" << statBoxes.size();
        //->graph()->setData(&data, false);
        box->setData(box->key(), boxData.min, boxData.firstQuartile, boxData.median, boxData.thirdQuartile, boxData.max);
        QCustomPlot *plot = box->parentPlot();
        plot->rescaleAxes(false);
        plot->replot();
    }

}

void GraphData::bindToWindow(PlotWindow *plotwin)
{
    Qt::GlobalColor color = plotwin->nextColor();
    QCustomPlot *plot = plotwin->getPlotWidget();
    QCPGraph *newGraph = plot->addGraph();
    newGraph->setPen(QPen(color));

    QCustomPlot *boxPlot = plotwin->getBoxPlotWidget();

    double boxPlotKey = plotwin->nextBoxPlotKey();

    newGraph->setData(&data,false);
    newGraph->setName(name);

    plots << plot;
/*
    QCPAxis *axis;
    if (expType == PING) {
        axis = plotwin->getPingAxis();
    } else if (expType == TCP) {
        axis = plotwin->getMbpsAxis();

    } else {
        qDebug() << "Error, unknown exptype";
        return;
    }
    */
    plotToStatbox[plotwin] = new QCPStatisticalBox(boxPlot->xAxis, boxPlot->yAxis);
    plotToGraph[plotwin] = newGraph;
    QCPStatisticalBox *newBox = plotToStatbox[plotwin];
    newBox->setPen(QPen(color));
    newBox->setName(name);
    boxPlot->addPlottable(newBox);
    newBox->setKey(boxPlotKey);
    statBoxes << newBox;
    plotwin->addGraph(this);
}

void GraphData::unbindWindow(PlotWindow *plotwin)
{
    QCustomPlot *plot = plotwin->getPlotWidget();
    QCPStatisticalBox *statbox = plotToStatbox.take(plotwin);
    QCustomPlot *boxPlot =plotwin->getBoxPlotWidget();
    QCPGraph *graph = plotToGraph.take(plotwin);
    boxPlot->removePlottable(statbox);
    plot->removeGraph(graph);
    plot->replot();
    boxPlot->replot();
    //statbox->deleteLater();
    statBoxes.removeAll(statbox);
    plots.removeAll(plot);

}

QString GraphData::expTypeString()
{
    if (expType == TCP) {
        return "TCP";
    }   else if (expType == PING) {
        return "PING";
    }
}

QString GraphData::ipTypeString()
{
    if (ipType == IPv4) {
        return "IPv4";

    } else if (ipType == IPv6) {
        return "IPv6";
    }
}

QString GraphData::getSrcAddr()
{
    return srcAddr;
}

QString GraphData::getDestAddr()
{
    return destAddr;
}

QString GraphData::getSrcNode()
{
    return srcNode;
}

QString GraphData::getDestNode()
{
    return destNode;
}

QString GraphData::getName()
{
    return name;
}

QString GraphData::getSrcProviderId()
{
    return srcProviderId;
}

QString GraphData::getDestProviderId()
{
    return destProviderId;
}

int GraphData::getsessionId()
{
    return sessionId;
}

BoxPlotData GraphData::generateBoxPlotData()
{
    /*
    QList<QCPData> values = data.values();
    qSort(values.begin(), values.end());

    int firstQuartileIndex = values.size() / 4;
    int thirdQuartileIndex = firstQuartileIndex * 3;
    int medianIndex = values.size() / 2;
    */BoxPlotData bpData;

    QList<double> values = sortedData.keys();
    if (values.size() == 0) {
        return BoxPlotData();
    }
    qDebug() << values;
    int medianIndex = values.size() /2;
    double median;
    if ((values.size() % 2) == 0) { //If there's  remainder, we need to take the average of the two middle values
        qDebug() << "Roudning median";
        median = (values[medianIndex - 1] + values[medianIndex]) / 2.0;
    } else {
        median = values[medianIndex];
    }
    int firstQuartileIndex = (values.size() / 4.0);
    int thirdQuartileIndex = (values.size() / 4.0) * 3.0;


    double firstQuartile = values[firstQuartileIndex];
    double thirdQuartile = values[thirdQuartileIndex];

    bpData.median = median;
    bpData.max = values.last();
    bpData.min = values.first();
    bpData.firstQuartile = firstQuartile;
    bpData.thirdQuartile = thirdQuartile;
    qDebug() << bpData.min << bpData.firstQuartile << bpData.median << bpData.thirdQuartile << bpData.max;
    return bpData;
}
