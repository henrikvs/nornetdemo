#include "mainwindow.h"
#include "regexwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "pingdialog.h"
#include "frontend.h"
#include "slicemanager.h"

#include <QDebug>
#include <string>
#include <iostream>
#include <stdio.h>

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
/*
std::string exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FrontEnd *frontEnd = new FrontEnd();


/*
    QQuickView *view = new QQuickView();
    //QDeclarativeView *view = new QDeclarativeView;
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *widget = QWidget::createWindowContainer(view, this);
    this->setCentralWidget(widget);
    view->setSource(QUrl("qrc:/images/map.qml"));
*/
    //layout->addWidget(widget);
    //this->setLayout(layout);
    //container->setFocusPolicy(Qt::TabFocus);


    //std::string res = exec("ls");
    //qDebug() << res.c_str();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSSL_triggered()
{

    qDebug() << "Test";
    Dialog *form = new Dialog(this);
    form->show();
}

void MainWindow::on_actionRegex_test_triggered()
{
    RegexWindow *form = new RegexWindow(this);
    form->show();
}

void MainWindow::on_actionPing_test_triggered()
{
    PingDialog *form = new PingDialog(this);
    form->show();
}
