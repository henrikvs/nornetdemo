#include "plotwindow.h"
#include "ui_plotwindow.h"

#include "graphdata.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);
    QCustomPlot *customPlot = ui->plotWidget;
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Mbps");
    customPlot->xAxis->setDateTimeFormat("mm:ss");
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);


    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // align the legend top left corner
    customPlot->legend->setVisible(true);
    static int id = 0;
    id++;
    ui->boxPlotWidget->legend->setVisible(true);

    ui->boxPlotWidget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // align the legend top left corner
    name = "Plot window " + QString::number(id);

    colorCycle << Qt::red;
    colorCycle << Qt::blue;
    colorCycle << Qt::black;
    colorCycle << Qt::green;
    colorCycle << Qt::darkRed;
    colorCycle << Qt::cyan;
    colorCycle << Qt::magenta;
    colorCycle << Qt::darkBlue;
    colorCycle << Qt::darkGreen;
    ui->plotWindowNameLabel->setText(name);

}

QCustomPlot *PlotWindow::getPlotWidget()
{
    return ui->plotWidget;
}

QCustomPlot *PlotWindow::getBoxPlotWidget()
{
    return ui->boxPlotWidget;
}

double PlotWindow::nextBoxPlotKey()
{
    static double key = 0;
    key+=0.6;
    return key;
}

void PlotWindow::addGraph(GraphData *graph)
{
    QListWidgetItem *item = new QListWidgetItem(graph->getName(), ui->experimentsWidget);

    itemToGraph[item] = graph;

    if (graph->expType == GraphData::PING) {
        pingGraphs++;
    } else if (graph->expType == GraphData::TCP) {
        mbpsGraphs++;
    }
    ui->plotWidget->yAxis->setLabel(createYAxisLabel());
    graphs << graph;
}

QString PlotWindow::getName()
{
    return name;
}

void PlotWindow::setName(QString name)
{
    this->name = name;
}
/*
QCPAxis *PlotWindow::getMbpsAxis()
{
    if (mbpsAxis == 0) {
        if (pingAxis == 0) {
            mbpsAxis = 1;
        } else if (pingAxis == 1) {
            mbpsAxis = 2;
        }
    }

    if (mbpsAxis == 1) {
        return ui->plotWidget->xAxis;
    } else if (mbpsAxis == 2) {
        return ui->plotWidget->yAxis;
    }
}

QCPAxis *PlotWindow::getPingAxis()
{
    if (pingAxis == 0) {
        if (mbpsAxis == 0) {
            pingAxis = 1;
        } else if (mbpsAxis == 1) {
            pingAxis = 2;
        }
    }

    if (pingAxis == 1) {
        return ui->plotWidget->xAxis;
    } else if (pingAxis == 2) {
        return ui->plotWidget->yAxis;
    }

}
*/
Qt::GlobalColor PlotWindow::nextColor()
{
    if (colorIndex > colorCycle.size()) {
        colorIndex = 0;

    }
    Qt::GlobalColor color= colorCycle[colorIndex];
    colorIndex++;
    return color;
}

PlotWindow::~PlotWindow()
{
    qDebug() << "Deconstructing!!!!!";
    foreach(GraphData *graph, graphs) {
        qDebug() << "Unbinding";
        graph->unbindWindow(this);
    }
    delete ui;
}

void PlotWindow::on_renamePlotButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter name"),tr("Enter name:"), QLineEdit::Normal, ui->plotWindowNameLabel->text(),
                                         &ok);
    if (ok && !text.isEmpty()) {
        ui->plotWindowNameLabel->setText(text);
        this->setWindowTitle(text);
        name = text;
    }
}

void PlotWindow::on_removeExperimentButton_clicked()
{


    ui->plotWidget->yAxis->setLabel(createYAxisLabel());
    qDebug() << "Trying to remove";
    QListWidgetItem *item = ui->experimentsWidget->currentItem();
    delete item;

    GraphData *graph = itemToGraph[item];

    if (graph->expType == GraphData::PING) {
        pingGraphs--;
    } else if (graph->expType == GraphData::TCP) {
        mbpsGraphs--;
    }
    ui->plotWidget->yAxis->setLabel(createYAxisLabel());

    graph->unbindWindow(this);
    graphs.removeAll(graph);
}

QString PlotWindow::createYAxisLabel()
{
    QStringList labels;

    if (pingGraphs > 0) {
        labels << "ms";
    }

    if (mbpsGraphs > 0) {
        labels << "Mbps";
    }
    return labels.join(" / ");
}
