#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

class GraphData;
namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    QCustomPlot* getPlotWidget();
    QCustomPlot* getBoxPlotWidget();
    double nextBoxPlotKey();
    void addGraph(GraphData *graph);
    QString getName();
    void setName(QString name);

    QCPAxis* getMbpsAxis();
    QCPAxis* getPingAxis();
    Qt::GlobalColor nextColor();
    QList<GraphData*> graphs;
    ~PlotWindow();

private slots:
    void on_renamePlotButton_clicked();

    void on_removeExperimentButton_clicked();

    void on_boxPlotAutoResize_clicked(bool checked);

    void on_graphAutoResize_clicked(bool checked);

    void handleExperimentSelected(QListWidgetItem *item);

private:
    Ui::PlotWindow *ui;
    QString name;
    QList<Qt::GlobalColor> colorCycle;
    QHash<QListWidgetItem*, GraphData*> itemToGraph;
    int colorIndex;
    int mbpsAxis;
    int pingAxis;
    int pingGraphs;
    int mbpsGraphs;
    QString createYAxisLabel();
    void setVisibleAxes();
};

#endif // PLOTWINDOW_H
