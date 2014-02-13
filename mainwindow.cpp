#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QDebug>
#include <string>
#include <iostream>
#include <stdio.h>
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
