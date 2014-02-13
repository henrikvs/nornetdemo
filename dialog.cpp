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
    qDebug() << name << domain;
    #ifdef Q_OS_WIN
    QString program = "C:/Users/Henrik/Downloads/plink.exe";
    #elif Q_OS_LINUX
    QString program = "ssh";
    #endif
    QStringList arguments;
    arguments << name + "@" + domain;
    myProcess.setParent(this);
    connect(&myProcess, SIGNAL(readyRead()), this, SLOT(programStarted()));
    myProcess.start(program, arguments);


}

void Dialog::programStarted()
{
    qDebug() << "It has started";
    ui->outputText->append(myProcess.readAll());
}

void Dialog::on_enterButton_clicked()
{
    QString command = ui->inputText->toPlainText();
    QByteArray array = command.toLocal8Bit();
    const char *chars = array.data();
    myProcess.write(chars);
    myProcess.write("\n");
    qDebug() << "Wrote";
    ui->inputText->setText("");
}
