#include "plotwindow.h"
#include "ui_plotwindow.h"

#include "graphdata.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow),
    colorIndex(0), mbpsAxis(0), pingAxis(0), pingGraphs(0), mbpsGraphs(0)
{
    ui->setupUi(this);

    connect(ui->experimentsWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(handleExperimentSelected(QListWidgetItem*)));
    QCustomPlot *customPlot = ui->plotWidget;

    ui->plotWidget->axisRect()->setRangeZoomAxes( ui->plotWidget->xAxis, NULL, ui->plotWidget->yAxis2, ui->plotWidget->yAxis);
        ui->plotWidget->axisRect()->setRangeDragAxes( ui->plotWidget->xAxis, ui->plotWidget->xAxis2, ui->plotWidget->yAxis, ui->plotWidget->yAxis2);


    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Mbps");
    customPlot->yAxis2->setLabel("MS");
    customPlot->xAxis->setDateTimeFormat("mm:ss");
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectAxes);
    connect(customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(handleAxisSelected()));


    customPlot->setProperty("autoresize", ui->graphAutoResize->isChecked());

    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // align the legend top left corner
    customPlot->legend->setVisible(true);
    static int id = 0;
    id++;
    ui->boxPlotWidget->legend->setVisible(true);
    ui->boxPlotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->boxPlotWidget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft | Qt::AlignTop); // align the legend top left corner
    ui->boxPlotWidget->setProperty("autoresize", ui->boxPlotAutoResize->isChecked());

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

    setVisibleAxes();
    //ui->plotWidget->yAxis->setLabel(createYAxisLabel());
    graphs << graph;
    if (ui->experimentsWidget->count() == 1) {
        handleExperimentSelected(item);
    }
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


    //ui->plotWidget->yAxis->setLabel(createYAxisLabel());
    qDebug() << "Trying to remove";
    QListWidgetItem *item = ui->experimentsWidget->currentItem();
    delete item;

    GraphData *graph = itemToGraph[item];

    if (graph->expType == GraphData::PING) {
        pingGraphs--;
    } else if (graph->expType == GraphData::TCP) {
        mbpsGraphs--;
    }

    setVisibleAxes();
    //ui->plotWidget->yAxis->setLabel(createYAxisLabel());

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

void PlotWindow::setVisibleAxes()
{
    if (pingGraphs > 0) {
        ui->plotWidget->yAxis2->setVisible(true);
    } else {
        ui->plotWidget->yAxis2->setVisible(false);
         //ui->plotWidget->axisRect()->setRangeDragAxes( ui->plotWidget->xAxis,  ui->plotWidget->yAxis);
    }

    if (mbpsGraphs > 0) {
        ui->plotWidget->yAxis->setVisible(true);
    } else {
        ui->plotWidget->yAxis->setVisible(false);
         //ui->plotWidget->axisRect()->setRangeDragAxes(ui->plotWidget->xAxis,  ui->plotWidget->yAxis2);
    }
}

void PlotWindow::on_boxPlotAutoResize_clicked(bool checked)
{
    ui->boxPlotWidget->setProperty("autoresize", checked);
}

void PlotWindow::on_graphAutoResize_clicked(bool checked)
{
    ui->plotWidget->setProperty("autoresize", checked);
}

void PlotWindow::handleExperimentSelected(QListWidgetItem *item)
{
    GraphData *graph = itemToGraph[item];
    ui->fromSiteEdit->setText(graph->getSrcNode());
    ui->fromSiteEdit->setCursorPosition(0);

    ui->toSiteEdit->setText(graph->getDestNode());
    ui->toSiteEdit->setCursorPosition(0);

    ui->fromIpEdit->setText(graph->getSrcAddr());
    ui->fromIpEdit->setCursorPosition(0);

    ui->toIpEdit->setText(graph->getDestAddr());
    ui->toIpEdit->setCursorPosition(0);
}

void PlotWindow::handleAxisSelected()
{
    QList<QCPAxis*> axes = ui->plotWidget->selectedAxes();

    if (axes.size() > 0) {
        foreach (QCPAxis *axis, axes) {
            if (axis == ui->plotWidget->yAxis) {
                ui->plotWidget->axisRect()->setRangeZoom(Qt::Vertical);
                ui->plotWidget->axisRect()->setRangeDragAxes(ui->plotWidget->xAxis, ui->plotWidget->yAxis);
                ui->plotWidget->axisRect()->setRangeZoomAxes(ui->plotWidget->xAxis2,  ui->plotWidget->yAxis);
            }
            if (axis == ui->plotWidget->yAxis2) {
                ui->plotWidget->axisRect()->setRangeZoom(Qt::Vertical);
                ui->plotWidget->axisRect()->setRangeDragAxes(ui->plotWidget->xAxis, ui->plotWidget->yAxis2);
                ui->plotWidget->axisRect()->setRangeZoomAxes(ui->plotWidget->xAxis2,  ui->plotWidget->yAxis2);
            }

            if (axis == ui->plotWidget->xAxis) {
                ui->plotWidget->axisRect()->setRangeZoom(Qt::Horizontal);
                ui->plotWidget->axisRect()->setRangeZoomAxes(ui->plotWidget->xAxis,  ui->plotWidget->yAxis2);
            }
        }
    } else {
        ui->plotWidget->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        ui->plotWidget->axisRect()->setRangeZoomAxes( ui->plotWidget->xAxis, NULL, ui->plotWidget->yAxis2, ui->plotWidget->yAxis);
        ui->plotWidget->axisRect()->setRangeDragAxes( ui->plotWidget->xAxis, ui->plotWidget->xAxis2, ui->plotWidget->yAxis, ui->plotWidget->yAxis2);
    }
}
