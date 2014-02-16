#include <QDebug>
#include <QObject>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
    connection.makeConnection(name, domain, password);
}

void Dialog::newOutput(QString output)
{
    qDebug() << "It has started";
    ui->outputText->append(output);
}

void Dialog::on_enterButton_clicked()
{
    QString command = ui->inputText->toPlainText();
    connection.execute(command);
    qDebug() << "Wrote";
    ui->inputText->setText("");
}
