#include <QDebug>
#include <QObject>
#include "dialog.h"
#include "ui_dialog.h" //test

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connection.addHost("henrik", "gatekeeper.nntb.no");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_connectButton_clicked()
{
    QString name = ui->nameText->toPlainText();
    QString domain = ui->domainText->toPlainText();
    QString password = ui->passwordText->toPlainText();

    connect(&connection, SIGNAL(outputSignal(QString)), this, SLOT(newOutput(QString)));
    connection.addHost(name, domain);
    connection.makeConnection();
}

void Dialog::newOutput(QString output)
{
    qDebug() << "It has started";
    ui->outputText->append(output);
}

void Dialog::on_enterButton_clicked()
{
    QString command = ui->inputText->toPlainText();
    connection.runCommand(command);
    qDebug() << "Wrote";
    ui->inputText->setText("");
}
